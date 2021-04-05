#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int numero_caracteres = 0;
int fin_hilo = 0;

//void* funcionHilo(void*);

void* funcionHilo(void* p){

    char letra[30];

    do{

        scanf("%s",letra);
        getchar();
        numero_caracteres+=strlen(letra);

    }while(letra[0] != 'q');

    fin_hilo = 1;

    pthread_exit(NULL);

    return p;
}

int main(){

    pthread_t idHilo;
    int error;

    error = pthread_create(&idHilo,NULL,funcionHilo,NULL);

    if(error != 0){
        perror("Error al crear el hilo.\n");
        exit(-1);
    }

    while(fin_hilo == 0){

        printf("Numero de caracteres introducidos: %i\n",numero_caracteres);
        sleep(1);
    }

    pthread_exit(NULL); // Asi al acabar el main thread los demas podran continuar

    return 0;
}
