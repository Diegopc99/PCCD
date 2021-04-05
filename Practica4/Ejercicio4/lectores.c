#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

typedef struct{
    int numero_lector;
}info;

sem_t semaforo;
sem_t semaforo_lectura[20];
sem_t semaforo_out[20];


void* funcionHilo(info* parametros){

    do{
        printf("[Lector %i] -> Esperando a intentar leer ...\n",parametros->numero_lector+1);

        sem_wait(&semaforo_lectura[parametros->numero_lector]); // Usamos este semaforo para suspender el proceso hasta que se lo depierte desde el main
        
        printf("[Lector %i] -> Intentando leer ...\n",parametros->numero_lector+1);

        sem_wait(&semaforo); // Usamos este semaforo para tener en cuenta el maximo numero de thrads que pueden ejecutarse a la vez, si se puede continua, si no se puede se suspende

        printf("[Lector %i] -> Leyendo ...\n",parametros->numero_lector+1);

        sem_wait(&semaforo_out[parametros->numero_lector]);

        printf("[Lector %i] -> Fin lectura\n",parametros->numero_lector+1);

        sem_post(&semaforo);

    }while(10);

    pthread_exit(NULL);

    return parametros;
}

int main(int args , char* argv[]){

    
    if(args != 3){
        printf("Numero de parametros introducido incorrecto.\n");
        exit(-1);
    }

    int N1 = atoi(argv[1]);  // Numero total de lectores
    int N2 = atoi(argv[2]);  // Numero max de lectores que pueden leer al mismo tiempo

    if(N2 > N1){
        printf("N1 tiene que ser mayor a N2.\n");
        exit(-1);
    }

    pthread_t idHilo[N1];

    sem_init(&semaforo,0,N2); //El segundo argumento (0) indica que el semaforo se comparte entre los hilos de un proceso y el tercero el valor de inicializacion del semaforo


    int error;
    int i = 0;

    info parametros[N1]; 

    for(i=0;i<N1;i++){

        parametros[i].numero_lector = i;

        error = pthread_create(&idHilo[i],NULL,(void*)funcionHilo,&parametros[i]);

        if(error != 0){
            perror("Error al crear el hilo.\n");
            exit(-1);
        }

        sem_init(&semaforo_lectura[i],0,0);
        sem_init(&semaforo_out[i],0,0);
    }

    int seleccion = 0;
    int lectura = 0;

    do{

        printf("1) Intentar leer\n");
        printf("2) Finalizar leer\n");
        printf("3) Salir\n");

        scanf("%i",&seleccion);

        if(seleccion !=1 && seleccion != 2 && seleccion !=3){
            printf("No ha seleccionado una opcion valida.\n");
            continue;
        }
        if(seleccion == 3){
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
            sem_post(&semaforo_out[lectura-1]);
        }

    }while(1);

    pthread_exit(NULL); // Asi al acabar el main thread los demas podran continuar

    return 0;
} 
 
