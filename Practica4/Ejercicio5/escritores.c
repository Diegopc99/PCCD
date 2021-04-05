#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

typedef struct{
    int numero_escritor;
}info;

sem_t semaforo;
sem_t semaforo_escritura[20];
sem_t semaforo_out[20];


void* funcionHilo(info* parametros){

    do{
        printf("[Escritor %i] -> Esperando a intentar escribir ...\n",parametros->numero_escritor+1);

        sem_wait(&semaforo_escritura[parametros->numero_escritor]); // Usamos este semaforo para suspender el proceso hasta que se lo depierte desde el main
        
        printf("[Escritor %i] -> Intentando escribir ...\n",parametros->numero_escritor+1);

        sem_wait(&semaforo); // Usamos este semaforo para tener en cuenta el maximo numero de thrads que pueden ejecutarse a la vez, si se puede continua, si no se puede se suspende

        printf("[Escritor %i] -> Escribiendo ...\n",parametros->numero_escritor+1);

        sem_wait(&semaforo_out[parametros->numero_escritor]);

        printf("[Escritor %i] -> Fin escritura\n",parametros->numero_escritor+1);

        sem_post(&semaforo);

    }while(10);

    pthread_exit(NULL);

    return parametros;
}

int main(int args , char* argv[]){

    
    if(args != 2){
        printf("Numero de parametros introducido incorrecto.\n");
        exit(-1);
    }

    int N3 = atoi(argv[1]);  // Numero total de escritores

    pthread_t idHilo[N3];

    sem_init(&semaforo,0,1); //El segundo argumento (0) indica que el semaforo se comparte entre los hilos de un proceso y el tercero el valor de inicializacion del semaforo


    int error;
    int i = 0;

    info parametros[N3]; 

    for(i=0;i<N3;i++){

        parametros[i].numero_escritor = i;

        error = pthread_create(&idHilo[i],NULL,(void*)funcionHilo,&parametros[i]);

        if(error != 0){
            perror("Error al crear el hilo.\n");
            exit(-1);
        }

        sem_init(&semaforo_escritura[i],0,0);
        sem_init(&semaforo_out[i],0,0);
    }

    int seleccion = 0;
    int lectura = 0;

    do{

        printf("1) Intentar escribir\n");
        printf("2) Finalizar escribir\n");
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
            printf("> introduzca el numero del escritor (de 1 a N3):");
            scanf("%i",&lectura);
            sem_post(&semaforo_escritura[lectura-1]);
        }

        if(seleccion == 2){

            printf("> introduzca el numero del escritor (de 1 a N3):");
            scanf("%i",&lectura);
            sem_post(&semaforo_out[lectura-1]);
        }

    }while(1);

    pthread_exit(NULL); // Asi al acabar el main thread los demas podran continuar

    return 0;
} 
 
 
