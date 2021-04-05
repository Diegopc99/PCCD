#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

typedef struct{
    int numero_escritor;
}infoEscritor;

typedef struct{
    int numero_lector;
}infoLector;

sem_t semaforo;
sem_t semaforo_escritura[20];
    sem_t sem_lectura;
    sem_t sem_escritura;
sem_t semaforo_lectura[20];
sem_t semaforo_lec_out[20];
sem_t semaforo_esc_out[20];
sem_t sem_global; 
sem_t sem_bloquea_escrit;
int escribiendo = 0,lectura = 0 ;

void* funcionHiloLector(infoLector* parametros){

    do{
        printf("[Lector %i] -> Esperando a intentar leer ...\n",parametros->numero_lector+1);

        sem_wait(&semaforo_lectura[parametros->numero_lector]); // Usamos este semaforo para suspender el proceso hasta que se lo depierte desde el main
        
        printf("[Lector %i] -> Intentando leer ...\n",parametros->numero_lector+1);

        sem_wait(&sem_global); //Bloqueamos para la S.C de cada lector

        lectura++;

        if(lectura == 1){  // Primer lector
            if(escribiendo != 0){
                sem_wait(&sem_global); // El primer lector se da cuenta de que se esta escribiendo y se bloquea hasta que se acabe de escribir, asi el resto de lectores se quedaran en la linea anterior
                    // Esperamos aqui hasta que el hilo escritor acabe y haga un post  
            }else{
                sem_wait(&sem_escritura); // Bloqueamos la escritura porque vamos leer
                sem_post(&sem_global); //Desbloqueamos la S.C porue no se esta escribiendo
            }
        }else{ //Resto de lectores

            sem_post(&sem_global); // Desbloqueamos la S.C porque no somos el primer lector
        }

            sem_wait(&sem_lectura);

            printf("[Lector %i] -> Leyendo ...\n",parametros->numero_lector+1);

            sem_wait(&semaforo_lec_out[parametros->numero_lector]);

            printf("[Lector %i] -> Fin lectura\n",parametros->numero_lector+1);

            sem_post(&sem_lectura);

            sem_wait(&sem_global); // Bloqueamos para la S.C

            lectura--;

            if(lectura == 0){  // Ultimo lector

                sem_post(&sem_escritura); // Desbloqueamos el escritor ya que no queda nadie leyendo y asi el puede escribir
                sem_post(&sem_global); //Desbloqueamos la S.C
            }else{
                sem_post(&sem_global);//Desbloqueamos la S.C
            }

        
    }while(10);

    pthread_exit(NULL);

    return parametros;
}


void* funcionHiloEscritor(infoEscritor* parametros){

    do{
        printf("[Escritor %i] -> Esperando a intentar escribir ...\n",parametros->numero_escritor+1);

        sem_wait(&semaforo_escritura[parametros->numero_escritor]); // Usamos este semaforo para suspender el proceso hasta que se lo depierte desde el main
        
        printf("[Escritor %i] -> Intentando escribir ...\n",parametros->numero_escritor+1);

        sem_wait(&sem_bloquea_escrit);
            escribiendo++;
        sem_post(&sem_bloquea_escrit);

            //sem_wait(&sem_lectura); 

        sem_wait(&sem_escritura); // Podra escribir si no queda nadie leyendo que bloqueara la escritura

        printf("[Escritor %i] -> Escribiendo ...\n",parametros->numero_escritor+1);

        sem_wait(&semaforo_esc_out[parametros->numero_escritor]);

        printf("[Escritor %i] -> Fin escritura\n",parametros->numero_escritor+1);

        sem_wait(&sem_bloquea_escrit);
            escribiendo--;
        sem_post(&sem_bloquea_escrit);

        if(escribiendo == 0){ // Ultimo lector

            //sem_post(&sem_lectura);
            sem_post(&sem_escritura); // Acabamos de escribir y desbloqueamos la escritura para que se pueda leer o que escriba otro
            sem_post(&sem_global); // Desbloqueamos la pausa que se hace en el la funcion lector cuando se cumple que se esta escribiendo, linea 41 asi pueden pasar a leer 

        }else{
           sem_post(&sem_escritura);// Acabamos de escribir y desbloqueamos la escritura
        }

    }while(10);

    pthread_exit(NULL);

    return parametros;
}

int main(int args , char* argv[]){

    
    if(args != 4){
        printf("Numero de parametros introducido incorrecto.\n");
        exit(-1);
    }

    int N1 = atoi(argv[1]);  // Numero total de lectores
    int N2 = atoi(argv[2]);  // Numero max de lectores que pueden leer al mismo tiempo
    int N3 = atoi(argv[2]);

    if(N2 > N1){
        printf("N1 tiene que ser mayor a N2.\n");
        exit(-1);
    }

    pthread_t idHiloEscritura[N3];
    pthread_t idHiloLectura[N1];

    //sem_init(&semaforo,0,N2); //El segundo argumento (0) indica que el semaforo se comparte entre los hilos de un proceso y el tercero el valor de inicializacion del semaforo


    int error;
    int i = 0;

    infoEscritor parametrosEscritor[N3]; 
    infoLector parametrosLector[N1];

    for(i=0;i<N3;i++){

        parametrosEscritor[i].numero_escritor = i;

        error = pthread_create(&idHiloEscritura[i],NULL,(void*)funcionHiloEscritor,&parametrosEscritor[i]);

        if(error != 0){
            perror("Error al crear el hilo.\n");
            exit(-1);
        }

        sem_init(&semaforo_escritura[i],0,0);
        sem_init(&semaforo_esc_out[i],0,0);

    }

    for(i=0;i<N1;i++){

        parametrosLector[i].numero_lector = i;

        error = pthread_create(&idHiloLectura[i],NULL,(void*)funcionHiloLector,&parametrosLector[i]);

        if(error != 0){
            perror("Error al crear el hilo.\n");
            exit(-1);
        }

        sem_init(&semaforo_lectura[i],0,0);
        sem_init(&semaforo_lec_out[i],0,0);
    }

    sem_init(&sem_escritura,0,1);
    sem_init(&sem_lectura,0,N2);
    sem_init(&sem_global,0,1);
    sem_init(&sem_bloquea_escrit,0,1);

    int seleccion = 0;
    int lectura = 0;

    do{

        printf("1) Intentar leer\n");
        printf("2) Finalizar leer\n");
        printf("3) Intentar escribir\n");
        printf("4) Finalizar escribir\n");
        printf("5) Salir\n");

        scanf("%i",&seleccion);

        if(seleccion !=1 && seleccion != 2 && seleccion !=3 && seleccion !=4 && seleccion !=5){
            printf("No ha seleccionado una opcion valida.\n");
            continue;
        }
        if(seleccion == 5){
            printf("Saliendo..");
            exit(0);
        }

        if(seleccion == 1){

            printf("> introduzca el numero del lector (de 1 a N1):");
            scanf("%i",&lectura);
            sem_post(&semaforo_lectura[lectura-1]);
        }

        if(seleccion == 2){

            printf("> introduzca el numero del lector (de 1 a N1):");
            scanf("%i",&lectura);
            sem_post(&semaforo_lec_out[lectura-1]);
        }

        if(seleccion == 3){

            printf("> introduzca el numero del escritor (de 1 a N3):");
            scanf("%i",&lectura);
            sem_post(&semaforo_escritura[lectura-1]);
        }

        if(seleccion == 4){

            printf("> introduzca el numero del escritor (de 1 a N3):");
            scanf("%i",&lectura);
            sem_post(&semaforo_esc_out[lectura-1]);
        }

    }while(1);

    pthread_exit(NULL); // Asi al acabar el main thread los demas podran continuar

    return 0;
} 
  
