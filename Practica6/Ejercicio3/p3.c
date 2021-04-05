#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>

struct msgbuf{
    long mtype;
    char *mtext;
}mensaje;

int main(int args,char *argv[]){

    if(args != 2){
        printf("Numero de parametros incorrecto.\n");
    }

    int id_queue = atoi(argv[1]);

    mensaje.mtext = "Primera prueba.\n";
    mensaje.mtype = 1;

    int status = 0;
    int tamano = 0;

    tamano = sizeof(mensaje.mtext);

    status = msgsnd(id_queue,&mensaje,tamano,0);

    if(status == -1){
        printf("Error al realizar el envio.\n");

    }else{
        printf("Mensaje enviado correctamente.\n");
    }
    return 0;
}
