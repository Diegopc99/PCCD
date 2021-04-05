#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

struct msgbuf{
    long mtype;
    int numtenedor;
}tenedor[6];

void comer(int numero_filosofo,int id_queue);

int main(int args,char *argv[]){ 

    int seleccion = 0;
    int numero_filosofo = 1;
    int id_queue = atoi(argv[1]);

    while(1){

        printf("1) Seleccionar el filosofo al que dar de comer.\n");
        printf("2) Salir.\n");

        scanf("%i",&seleccion);

        switch(seleccion){

            case 1:

                printf("Introduce el numero del filosofo: ");
                scanf("%i",&numero_filosofo);
                comer(numero_filosofo,id_queue);
                break;
            
            case 2:
                printf("Saliendo...\n");
                exit(0);
            
            default:
                printf("Opcion no valida.\n");
                break;
        }
    }

    return 0;
}

void comer(int numero_filosofo,int id_queue){

    printf("El filosofo %i va a intentar coger el tenedor.\n",numero_filosofo);

    msgrcv(id_queue,&tenedor[numero_filosofo],sizeof(int),numero_filosofo,0); //al mandar como type numero filosofo quitamos el mensaje con ese mismo identificador

    printf("El filosofo %i ha cogido el tenedor %i.\n",numero_filosofo,numero_filosofo);

    int numero_tenedor = numero_filosofo -1; 

    if(numero_tenedor == 0){  // Caso del primer filosofo, que a un lado tiene el tenedor 1 y al otro el 5
        numero_tenedor = 5;
    }

    printf("El filosofo %i va a intentar coger el tenedor.\n",numero_filosofo);

    msgrcv(id_queue,&tenedor[numero_tenedor],sizeof(int),numero_tenedor,0); // Si no puede coger el mensaje, espera a que vuelva a estar el mensaje que busca en el buzon

    printf("El filosofo %i ha cogido el tenedor %i.\n",numero_filosofo,numero_tenedor);

    printf("El filosofo %i esta comiendo... \n",numero_filosofo);

    sleep(10);

    printf("El filosofo %i ha acabado de comer y devuelve los dos tenedores.\n",numero_filosofo);

    tenedor[numero_filosofo].mtype = numero_filosofo;
    tenedor[numero_filosofo].numtenedor = numero_filosofo;

    tenedor[numero_tenedor].mtype = numero_tenedor;
    tenedor[numero_tenedor].numtenedor = numero_tenedor;

    msgsnd(id_queue,&tenedor[numero_filosofo],sizeof(int),IPC_NOWAIT); // Mandamos el mensaje del tenedor al buzon para que otros filosofos puedan cogerlos
    msgsnd(id_queue,&tenedor[numero_tenedor],sizeof(int),IPC_NOWAIT);

    return;
}
