#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

typedef struct{
    int quiere_p;
}zona;

int main(int args,char *arg[]){ //Ejecutar programa con sudo para que pueda acceder a la memoria

    key_t key;
    zona *memzone;

    size_t size = sizeof(int); //Solo ocupa un entero
    int id_memoria = 0;
    int turno = atoi(arg[1]);

    key = ftok("/home/diego/Escritorio/PCCD/Practica3",'2');
    
    id_memoria = shmget(key,size,IPC_CREAT|0666);
    if(id_memoria == -1){
        printf("No se ha creado el fragmento de memoria.\n");
        return -1;
    }

    memzone = shmat(id_memoria,NULL,0); //NULL hace que el so asigne la mejor direccion de pagina no usada para el segmento

    if(memzone == (-1)){
        printf("Zona de memoria no asignada.");
        return -1;
    }

    memzone->quiere_p = 0;

    printf("Pulse ENTER para entrar en la Seccion Critica y ENTER para salir de ella: \n");

    while(10){
        
        printf("Caminando por mi habitacion\n");
        //printf("Pulse ENTER para entrar en la Seccion Critica y ENTER para salir de ella: \n");
        getchar();
        printf("Intentando entrar en la Seccion Critica...\n");
        if(memzone->quiere_p == turno){ //
            printf("Dentro de la Seccion Critica.\n");
            getchar();
            printf("Saliendo de la Seccion Critica\n");
            if(memzone->quiere_p == 0){
                memzone->quiere_p = 1;
            }else{
                memzone->quiere_p = 0;
            }
            printf("Pulsador accionado\n");
        }else{
            printf("Puerta cerrada\n");
        }

    }

    shmdt(memzone); //Separa el espacio de memoria creado del espacio de memoria del propio proceso, al contrario que shmat

    //shmctl(id_memoria,IPC_RMID,NULL);// Libera memoria compartida creada con shmget
                                     // IPC_RMID libera la memoria cuando se haya desvinculado de un proceso con shmdt 

    return 0;
} 
