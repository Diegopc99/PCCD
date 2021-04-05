 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>


void* thread_carvajal(void* p){

    printf("Carvajal    ");

    pthread_exit(NULL);

    return p;
}

void* thread_ramos(void* p){

    printf("Ramos    ");

    pthread_exit(NULL);

    return p;
}

void* thread_pique(void* p){

    printf("Pique    ");

    pthread_exit(NULL);

    return p;
}

void* thread_thiago_silva(void* p){

    printf("Thiago    ");
    printf("Silva   ");

    pthread_exit(NULL);

    return p;
}

int main(int args , char* argv[]){

    pthread_t Carvajal,Ramos,Pique,Thiago_Silva;

    int error;

    printf("De Gea      ");

    error = pthread_create(&Carvajal,NULL,(void*)thread_carvajal,NULL);

        if(error != 0){
            perror("Error al crear el hilo de Carvajal.\n");
            exit(-1);
        }
    
    error = pthread_create(&Ramos,NULL,(void*)thread_ramos,NULL);

        if(error != 0){
            perror("Error al crear el hilo de Ramos.\n");
            exit(-1);
        }

    error = pthread_create(&Pique,NULL,(void*)thread_pique,NULL);

        if(error != 0){
            perror("Error al crear el hilo de Pique.\n");
            exit(-1);
        }

    printf("Jordi Alba      ");

    error = pthread_create(&Thiago_Silva,NULL,(void*)thread_thiago_silva,NULL);

        if(error != 0){
            perror("Error al crear el hilo de Pique.\n");
            exit(-1);
        }

    pthread_join(Carvajal,NULL);
    pthread_join(Ramos,NULL);
    pthread_join(Pique,NULL);

    printf("Busquets    ");
    printf("Isco    ");
    printf("Aspas   ");

    pthread_join(Thiago_Silva,NULL);

    printf("Morata\n");

    pthread_exit(NULL); // Asi al acabar el main thread los demas podran continuar

    return 0;
} 