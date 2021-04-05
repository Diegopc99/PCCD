#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>

struct msgbuf{
    long mtype;
    int numtenedor;
}tenedor[6];

int main(int args,char *argv[]){

    if(args != 2){
        printf("Numero de parametros incorrecto.\n");
        exit(-1);
    }

    int id_queue = atoi(argv[1]); 
    int i = 0;

    for(i=1;i<6;i++){
        tenedor[i].mtype = i;
        tenedor[i].numtenedor = i;

        int status = 0;
        int tamano = 0;

        tamano = sizeof(tenedor[i].numtenedor);

        status = msgsnd(id_queue,&tenedor[i],tamano,IPC_NOWAIT);

        if(status == -1){
            printf("Error al anadir el tenedor a la cola.\n");

        }else{
            printf("Tenedor anadido correctamente a la cola.\n");
        }
    }

    printf("Todos los tenedores anadidos a la cola correctamente.\n");

    return 0;
}
 
