#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int args, char *argv[]){

    if(args != 2){
        printf("Parametros incorrectos.\n");
        exit(-1);
    }

    int id_queue = atoi(argv[1]);
    int status = 0;

    status = msgctl(id_queue,IPC_RMID,NULL);

    if(status == -1){
        printf("Error al eliminar la cola.\n");
        exit(-1);
    }

    printf("Eliminada la cola con identificador: %i\n",id_queue);

    return 0;
} 
