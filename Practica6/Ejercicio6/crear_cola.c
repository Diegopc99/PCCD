#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int args, char *argv[]){

    int id_queue = 0;

    key_t key = ftok("/home/diego/Escritorio/PCCD/Practica6",20);

    if(key == -1){
        printf("Error en la generacion de la clave. \n");
        exit(-1);
    }

    id_queue = msgget(key,IPC_CREAT|0777);

    if(id_queue == -1){
        printf("Error al generar la cola de mensajes.\n");
        exit(-1);
    }

    printf("ID cola de mensajes: %i\n",id_queue);

    return 0;
}
 
