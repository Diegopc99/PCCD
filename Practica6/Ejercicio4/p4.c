#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>

typedef struct msgbuf{
    long mtype;
    char *mtext;
}mensaje;

int main(int args,char *argv[]){

    if(args != 2){
        printf("Numero de parametros incorrecto.\n");
    }

    int id_queue = atoi(argv[1]);

    mensaje msg;
    msg.mtype = 1;

    int status = 0;
    int tamano = 0;

    tamano = sizeof(msg.mtext);

    status = msgrcv(id_queue,&msg,tamano,0,0);

    if(status == -1){
        printf("Error al recibir el mensaje.\n");
        exit(-1);

    }else{
        printf("Mensaje recibido correctamente.\n");
    }
    
    return 0;
} 
