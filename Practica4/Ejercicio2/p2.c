#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct{
    char* texto;
    int posicion;
}info;

void* funcionHilo(info* args){

    printf("Texto: %s ; Posicion: %i \n",args->texto, args->posicion);

    pthread_exit(NULL);

    return args;
}

int main(int args , char* argv[]){

    pthread_t idHilo[args-1];
    info parametros[args-1]; 

    int error;
    int i = 0;

    for(i=1;i<args;i++){

        parametros[i].texto = argv[i];
        parametros[i].posicion = i;

        error = pthread_create(&idHilo[i],NULL,(void*)funcionHilo,&parametros[i]);

        if(error != 0){
            perror("Error al crear el hilo.\n");
            exit(-1);
        }
    }

    pthread_exit(NULL); // Asi al acabar el main thread los demas podran continuar

    return 0;
} 
