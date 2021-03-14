#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

typedef struct{
    int entero;
}zona;

int main(int args,char *arg[]){ //Ejecutar programa con sudo para que pueda acceder a la memoria

    key_t key;
    zona *memzone;

    size_t size = sizeof(int); //Solo ocupa un entero
    int id_memoria = 0;

    key = ftok("/home/diego/Escritorio/PCCD/Practica3",'2');
    
    id_memoria = shmget(key,size,IPC_CREAT|0666);

    if(shmctl(id_memoria,IPC_RMID,NULL) == -1){ // Libera memoria compartida creada con shmget
                                                 // IPC_RMID libera la memoria cuando se haya desvinculado de un proceso con shmdt 
        printf("No se ha podido liberar memoria.\n");
    }
    return 0;
}
 