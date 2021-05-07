#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/time.h>
#include <string.h>

struct mensaje{
    long mtype;
    int id_nodo;
    int ticket;
    int prio_proceso;
};

int receive(int tipo_cola,int *id_nodo_origen,int *ticket,int *prio_proceso_origen);
int send(int tipo_cola,int id_nodo_destino,int id_nodo_origen,int ticket,int prio_proceso);
void* hiloProceso(void *);
void* hiloReceptor(void* args);
int crearCSV();
long restaTiempo(struct timeval s1,struct timeval s2);
struct timeval TimeStamp();
int rellenarCSV(int numero_proceso,long t_espera_SC,long t_fin_ejecucion);
int crear_procesos(int num_admin,int num_anul,int num_pre_reservas, int num_pagos,int num_eventos,int num_gradas);
int calcula_maximo(int m1,int m2);

int mi_ticket = 0;
int mi_id;
int id_nodos_pend[100];
int num_pendientes = 0;
int N = 0;
int idColaRequest;
int idColaReply;
int quiero = 0;
int max_ticket = 0;
int Nprocesos = 0;
int REQUEST = 1;
int REPLY = 2;
int id_nodos[1000];
int lectura = 0,escribiendo = 0;
int mi_prio = 0;
int num_lectores = 0;
int SC = 0;
int n_anulaciones = 0;
int n_pagos = 0;
int n_pre_reservas = 0;
int n_administracion = 0;
int n_lectores = 0;
int cola_lectores = 0;
int lectoresen_SC = 0;


char nombre_fichero[50];

struct timeval tiempo_inic_ejec;

sem_t sem_procesos;
sem_t sem_solicita_SC;
sem_t sem_sale_SC;
sem_t sem_resto_procesos;
sem_t sem_crear_hilos;
sem_t sem_block_lectura;
sem_t sem_block_SC;
sem_t sem_block_ticket;
sem_t sem_block_quiero;

sem_t sem_anulaciones;
sem_t sem_pagos;
sem_t sem_pre_reservas;
sem_t sem_admin;
sem_t sem_lectores;

void* hiloReceptor(void* args){

    int id_nodo_origen = 0;
    int ticket_origen = 0;
    int prio_proceso_origen = 0;

    do{
        //recibimos los tickets de cada nodo
        receive(REQUEST,&id_nodo_origen,&ticket_origen,&prio_proceso_origen); // Al pasarlo como puntero por parametro podemos mantener los cambios
        
        if(prio_proceso_origen == -1){ //Proceso origen solicita  habilitar lectura
            sem_wait(&sem_block_lectura);
                lectura = 1;
            sem_post(&sem_block_lectura);

        }else{ // Proceso origen solicita habilitar escritura 

            sem_wait(&sem_block_ticket);
            max_ticket = calcula_maximo(max_ticket,ticket_origen);
            sem_post(&sem_block_ticket);

            sem_wait(&sem_block_lectura);
                if(prio_proceso_origen != 5){
                    lectura = 0;
                }

            //if(mi_ticket > ticket_origen){
            if( SC == 0 && ((!quiero) || (ticket_origen < mi_ticket) || ((ticket_origen == mi_ticket) && (id_nodo_origen < mi_id)))){
                sem_post(&sem_block_lectura);
                //enviamos un reply al nodo si el ticket es menor
                send(REPLY,id_nodo_origen,mi_id,0,0); // idColaReply
                printf("Enviada confirmacion al nodo %i\n",id_nodo_origen);

            }else{
                sem_post(&sem_block_lectura);
                id_nodos_pend[num_pendientes++] = id_nodo_origen;   
            }
        }
        
    }while(1);

    pthread_exit(NULL);

    return args;
}

void inicializacion(int N,int *id_nodos,int num_admin,int num_anul,int num_pre_reservas,int num_pagos,int num_eventos,int num_gradas){

    printf("LLegamos a inicializacion..\n");
    //////////////// CREAMOS EL HILO DEL PROCESO RECEPTOR ///////////////

    pthread_t hilo_receptor;
    pthread_create(&hilo_receptor,NULL,hiloReceptor,"");

    ////////////////////////////////////////////////////////////////////

    ///////////////////// CREAMOS EL FICHERO CSV ///////////////////////

    crearCSV();
    tiempo_inic_ejec = TimeStamp(); // Valor en microsegundos

    crear_procesos(num_admin,num_anul,num_pre_reservas,num_pagos,num_eventos,num_gradas);

    ///////////////////////////////////////////////////////////////////

    /////////////////// INICIALIZAMOS LOS SEMAFOROS //////////////////

    sem_init(&sem_procesos,0,0);
    sem_init(&sem_solicita_SC,0,0);
    sem_init(&sem_sale_SC,0,0);
    

    sem_init(&sem_resto_procesos,0,1);
    sem_init(&sem_crear_hilos,0,1);
    sem_init(&sem_block_lectura,0,1);
    sem_init(&sem_block_SC,0,1);
    sem_init(&sem_block_ticket,0,1);
    sem_init(&sem_block_quiero,0,1);

    sem_init(&sem_anulaciones,0,0);
    sem_init(&sem_pagos,0,0);
    sem_init(&sem_pre_reservas,0,0);
    sem_init(&sem_admin,0,0);
    sem_init(&sem_lectores,0,0);

    printf("Salimos del inicializador..\n");
    //////////////////////////////////////////////////////////////////

    return;
}

int calcula_maximo(int m1,int m2){
    if(m2 > m1){
        return m2;
    }else{
        return m1;
    }
}

int send(int tipo_cola,int id_nodo_destino,int id_nodo_origen,int ticket,int prio_proceso){
        
    struct mensaje mensaje;

    mensaje.mtype = tipo_cola; // Para que asi el destinatario pueda recojer sus mensajes
    mensaje.id_nodo = id_nodo_origen;
    mensaje.ticket = ticket;
    mensaje.prio_proceso = prio_proceso;

    msgsnd(id_nodo_destino,(struct msgbuf *)&mensaje,sizeof(mensaje.id_nodo)*3,0);

    return 0;
}

int receive(int tipo_cola,int *id_nodo_origen,int *ticket,int *prio_proceso_origen){  //Utilizamos punteros para poder actualizar las variables con los datos recibidos

    struct mensaje mensaje;

    msgrcv(mi_id,(struct msgbuf *)&mensaje,sizeof(mensaje.id_nodo)*3,tipo_cola,0);

    *id_nodo_origen = mensaje.id_nodo;
    *ticket = mensaje.ticket;
    *prio_proceso_origen = mensaje.prio_proceso;

    return 0;
}

int crear_procesos(int num_admin,int num_anul,int num_pre_reservas, int num_pagos,int num_eventos,int num_gradas){

    pthread_t hilo_procesos[100];
    int i = 0;
    int tipo_proceso = 0;

    tipo_proceso = 0;
    for(i=0;i<num_anul;i++){

        pthread_create(&hilo_procesos[i],NULL,hiloProceso,(void *)&tipo_proceso);
        sem_wait(&sem_crear_hilos); //Semaforo para controlar que el hilo actualice su tipo correctamente
    }

    tipo_proceso = 1;
    for(i=0;i<num_pagos;i++){

        pthread_create(&hilo_procesos[i],NULL,hiloProceso,(void *)&tipo_proceso);
        sem_wait(&sem_crear_hilos); //Semaforo para controlar que el hilo actualice su tipo correctamente
    }

    tipo_proceso = 2;
    for(i=0;i<num_pre_reservas;i++){

        pthread_create(&hilo_procesos[i],NULL,hiloProceso,(void *)&tipo_proceso);
        sem_wait(&sem_crear_hilos); //Semaforo para controlar que el hilo actualice su tipo correctamente
    }

    tipo_proceso = 3;
    for(i=0;i<num_admin;i++){

        pthread_create(&hilo_procesos[i],NULL,hiloProceso,(void *)&tipo_proceso);
        sem_wait(&sem_crear_hilos); //Semaforo para controlar que el hilo actualice su tipo correctamente
    }

    tipo_proceso = 4;
    for(i=0;i<num_eventos;i++){

        pthread_create(&hilo_procesos[i],NULL,hiloProceso,(void *)&tipo_proceso);
        sem_wait(&sem_crear_hilos); //Semaforo para controlar que el hilo actualice su tipo correctamente
    }

    tipo_proceso = 5;
    for(i=0;i<num_gradas;i++){

        pthread_create(&hilo_procesos[i],NULL,hiloProceso,(void *)&tipo_proceso);
        sem_wait(&sem_crear_hilos); //Semaforo para controlar que el hilo actualice su tipo correctamente
    }

    return 0;
}

void* hiloProceso(void *tipo_proceso){

    int proceso = *((int*)tipo_proceso);

    int proceso_mi_prio = 0;
    int *contador_num_procesos;
    int *plectura = &lectura;
    int *pcola_lectores = &cola_lectores;
    int *plectoresen_SC = &lectoresen_SC;

    sem_t* sem_block_contador;
    sem_t* sem_proceso;

    char Proceso[6][20] = {
        
        "Anulaciones",
        "Pagos",
        "Pre-reservas",
        "Administracion",
        "Eventos",
        "Gradas"
    };

     //Para que le de tiempo al main a empezar a esperar haciendo el wait (tiempo para la inicializacion de todos los hilos)

    if(proceso == 0){ ///ANULACIONES
        proceso_mi_prio = 1;
        contador_num_procesos = &n_anulaciones; // Cada contador _num_procesos modifica el valor de la variable global n_algo asi sabemos cuantos procesos hay de cada tipo y cuando se despachan
        sem_proceso = &sem_anulaciones;
    }
    if(proceso == 1){//PAGOS
        proceso_mi_prio = 2;
        contador_num_procesos = &n_pagos;
        sem_proceso = &sem_pagos;
    }
    if(proceso == 2){//PRE-RESERVAS
        proceso_mi_prio = 3;
        contador_num_procesos = &n_pre_reservas;
        sem_proceso = &sem_pre_reservas;
    }
    if(proceso == 3){//ADMINISTRACION
        proceso_mi_prio = 4;
        contador_num_procesos = &n_administracion;
        sem_proceso = &sem_admin;
    }
    if(proceso == 4){//EVENTOS
        proceso_mi_prio = 5;
        contador_num_procesos = &n_lectores;
        sem_proceso = &sem_lectores;
        
    }
    if(proceso == 5){//GRADAS
        proceso_mi_prio = 5;
        contador_num_procesos = &n_lectores;
        sem_proceso = &sem_lectores;
    }

    printf("Creado proceso de %s con prio %i\n",Proceso[proceso],proceso_mi_prio);

    sem_post(&sem_crear_hilos);

    sleep(10);

    if(proceso_mi_prio != 5){
        lectura = 0;
    }

    //// INCREMENTAMOS CONTADOR DE PROCESOS PARA GUARDAR LA VARIABLES GLOBALES n_ALGO/////

    *contador_num_procesos = *contador_num_procesos + 1;

    /////////////////////////////////////////////

    if(proceso_mi_prio != 5){
        sem_post(&sem_solicita_SC);
    }

    if(proceso_mi_prio == 5 && lectoresen_SC == 1 && lectura == 1){
        //Si se ha habilitado la lectura y hay lectores en SC hacemos el if para que no se queden esperando al main en la siguiente condicion y continuen a su seccion critica
    }else if(proceso_mi_prio == 5){

        *pcola_lectores = *pcola_lectores+1; // Con esto contamos todos los de lectura y los dejamos esperando al main
        sem_wait(sem_proceso); // Esperamos a que el main despierte a los de lectura
        *pcola_lectores = *pcola_lectores-1;

    }else{ //Procesos de escritura
        sem_wait(sem_proceso); // Esperamos a que el main despierte a los de escritura
    }

    ///////////// ZONA DE SECCION CRITICA ///////////////

    printf("Proceso de %s entrando en SECCION CRITICA\n",Proceso[proceso]);

    if(proceso_mi_prio == 5){ //Si en el if anterior se dejo pasar a un proceso de lectura habilitamos la lectura global
        *plectura = 1;
        *plectoresen_SC = 1; //Avisamos al resto de lectores de que un lector esta ejecutando su SECCION CRITICA
    }

    *contador_num_procesos = *contador_num_procesos - 1; /// Contador para que el main sepa cuantos procesos de cada tipo quedan y les pueda dar paso

    if(proceso_mi_prio == 5 && cola_lectores != 0){
        sem_post(&sem_solicita_SC);
    }

    sleep(10);

    printf("Proceso de %s saliendo de SECCION CRITICA\n",Proceso[proceso]);

    *plectoresen_SC = 0;
    sem_post(&sem_sale_SC); /// Salimos de la SECCION CRITICA

    ////////////// SALIMOS DE SECCION CRITICA /////////////

        //long t_espera_SC = restaTiempo(t_inicio_SC,tiempo_inic_ejec);
        //struct timeval t_fin_ejecucion = TimeStamp();

        //rellenarCSV(proceso,t_espera_SC,t_fin_ejecucion.tv_usec);
    
    printf("Proceso de %s FINALIZADO\n",Proceso[proceso]);    

    pthread_exit(NULL);

    //free(numero_proceso); //Liberamos la memoria creada con malloc en la inicializacion

    return tipo_proceso;
}

int main(int args,char *argv[]){  // ./proceso-hilos mi_id nodos

    //pthread_t idHilo;

    mi_id = atoi(argv[1]);
    N = atoi(argv[2]);
    //Nprocesos = atoi(argv[3]);
    int num_admin = atoi(argv[3]);
    int num_anul = atoi(argv[4]);
    int num_pre_reservas = atoi(argv[5]);
    int num_pagos = atoi(argv[6]);
    int num_eventos = atoi(argv[7]);
    int num_gradas = atoi(argv[8]);
    //id_nodos[N-1];
    int id_aux = 0;
    int prio_proceso_aux = 0;
    int ticket_aux = 0;

    for(int i=0;i<N-1;i++){  //Utilizamos los id de colas introducidos como id de nodo
        id_nodos[i] = atoi(argv[i+9]);
    }

    inicializacion(N,&id_nodos[0],num_admin,num_anul,num_pre_reservas,num_pagos,num_eventos,num_gradas);

    printf("mi_id: %i\n",mi_id);
    printf("id_nodo 1: %i\n",id_nodos[0]);
    printf("%i\n",id_nodos[1]);
    printf("%i\n",id_nodos[2]);

    do{

        sem_wait(&sem_solicita_SC);

        printf("Un proceso ha solicitado la SECCION CRITICA\n");

        if(n_anulaciones > 0){
            mi_prio = 1;
        }else if(n_pagos > 0){
            mi_prio = 2;
        }else if(n_pre_reservas > 0){
            mi_prio = 3;
        }else if(n_administracion > 0){
            mi_prio = 4;
        }else if(n_lectores > 0){
            mi_prio = 5;
        }

        int i = 0;

        sem_wait(&sem_block_quiero);
        quiero = 1;
        sem_post(&sem_block_quiero);

        sem_wait(&sem_block_ticket);
        mi_ticket = max_ticket+1; //Sustituimos el rand() por el maximo ticket
        sem_post(&sem_block_ticket);

        for(i=0;i<N-1;i++){ // No metemos al primero del array porque es el propio nodo
            
            //enviamos a todos los nodos los tickets
            send(REQUEST,id_nodos[i],mi_id,mi_ticket,mi_prio);
            printf("Solicitud enviada al nodo %i\n",id_nodos[i]);
        }

        for(i=0;i<N-1;i++){
            //recibimos los tickets de todos los nodos
            receive(REPLY,&id_aux,&ticket_aux,&prio_proceso_aux);  // idColaReply //////////Cuidado
            printf("Confirmacion recibida del nodo %i\n",id_aux);
        }

        ///////////////////SECCION CRITICA///////////////

        sem_wait(&sem_block_SC);
        SC = 1;
        sem_post(&sem_block_SC);

        printf("ENTRANDO EN SECCION CRITICA EN EXCLUSION MUTUA\n");
            
        if(n_anulaciones > 0){
            printf("Notificacion enviada a proceso de ANULACIONES\n");
            sem_post(&sem_anulaciones);
        }else if(n_pagos > 0){
            printf("Notificacion enviada a proceso de PAGOS\n");
            sem_post(&sem_pagos);
        }else if(n_pre_reservas > 0){
            printf("Notificacion enviada a proceso de PRE-RESERVAS\n");
            sem_post(&sem_pre_reservas);
        }else if(n_administracion > 0){
            printf("Notificacion enviada a proceso de ADMINISTRACION\n");
            sem_post(&sem_admin);
        }else if(n_lectores > 0){
            printf("Notificacion enviada a procesos de LECTORES\n");
            for(int k = 0;k< n_lectores;k++){
                sem_post(&sem_lectores);
            }

            printf("Notificados resto de NODOS para LEER\n");
            for(int n = 0;n<N-1;n++){
                send(REQUEST,id_nodos[n],mi_id,mi_ticket,-1);
            }
        }

        sem_wait(&sem_sale_SC);// Esperamos a que el proceso acabe de ejecutar su SC
        
        sem_wait(&sem_block_SC);
        SC = 0;
        sem_post(&sem_block_SC);

        sem_wait(&sem_block_quiero);
        quiero = 0;
        sem_post(&sem_block_quiero);

        for(i=0;i<num_pendientes;i++){
            //enviamos a cada nodo un reply de que hemos pasado la seccion critica
            send(REPLY,id_nodos_pend[i],mi_id,0,0); //idColaReply
            printf("Respuesta enviada al nodo %i\n",id_nodos_pend[i]);
        }

        num_pendientes = 0;

        sem_wait(&sem_block_lectura);
        lectura = 0;
        sem_post(&sem_block_lectura);

    }while(1);

    return 0;
} 

long restaTiempo(struct timeval s1,struct timeval s2){

    long resta = s1.tv_sec - s2.tv_sec;

    return resta;
}

int crearCSV(){

    FILE *archivo;
    strcpy(nombre_fichero,"DATA");
        char buffer_mi_ID[20];
        sprintf(buffer_mi_ID,"%d",mi_id); 
    strcat(nombre_fichero,buffer_mi_ID);
    strcat(nombre_fichero,".csv");

    printf("%s\n",nombre_fichero);

    archivo = fopen(nombre_fichero,"w");
    fprintf(archivo,"ID_NODO,NUMERO_PROCESO,TIEMPO_ESPERA_SC,TIEMPO_EJECUCION\n");
    fclose(archivo);

    return 0;
}
 
int rellenarCSV(int numero_proceso,long t_espera_SC,long t_fin_ejecucion){

    FILE *archivo;

    long tiempo_ejecucion = t_fin_ejecucion - tiempo_inic_ejec.tv_usec;

    archivo = fopen(nombre_fichero,"a");
    fprintf(archivo,"%i,%i,%ld,%ld\n",mi_id,numero_proceso,t_espera_SC,tiempo_ejecucion);
    fclose(archivo);

    return 0;
}

struct timeval TimeStamp(){

    struct timeval current_time;
    gettimeofday(&current_time,NULL);

    return current_time;

}