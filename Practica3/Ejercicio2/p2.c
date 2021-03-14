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

    int numero = atoi(arg[1]);

    key = ftok("/home/diego/Escritorio/PCCD/Practica3/Ejercicio2",'2');
    
    id_memoria = shmget(key,size,IPC_CREAT|0666);

    memzone = shmat(id_memoria,NULL,0); //NULL hace que el so asigne la mejor direccion de pagina no usada para el segmento

    printf("Segmento de memoria con ID: %d\n",id_memoria);
    printf("Valor de la zona de memoria: %p\n",memzone);

    memzone -> entero = numero;
    //memmove(sh_mem,numero,strlen(numero));//Copia la memoria de numero a sh_mem memmove(dest,orig,size)

    printf("%i \n",memzone->entero);

    shmdt(memzone); //Separa el espacio de memoria creado del espacio de memoria del propio proceso, al contrario que shmat

    shmctl(id_memoria,IPC_RMID,NULL);// Libera memoria compartida creada con shmget
                                     // IPC_RMID libera la memoria cuando se haya desvinculado de un proceso con shmdt 

    return 0;
}
 
