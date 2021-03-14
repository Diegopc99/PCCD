#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

typedef struct{
    int quiere_p;
    int quiere_q;
}zona;

int main(int args,char *arg[]){ //Ejecutar programa con sudo para que pueda acceder a la memoria

    key_t key;
    zona *memzone;

    size_t size = sizeof(int); //Solo ocupa un entero
    int id_memoria = 0;
    char letra_proceso = *arg[1];

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

    memzone->quiere_p = 0; // 0 = puerta abierta  |   1 = puerta cerrada
    memzone->quiere_q = 0;

    printf("Pulse ENTER para entrar en la Seccion Critica y ENTER para salir de ella: \n");

    while(10){
        
        printf("Caminando por mi habitacion\n");
        //printf("Pulse ENTER para entrar en la Seccion Critica y ENTER para salir de ella: \n");
        getchar();
        printf("Dentro del pasillo\n");
        

        if(letra_proceso == 'p'){
                getchar();
                memzone->quiere_q = 1;
                printf("He accionado el pulsador\n");
                getchar();
                printf("Intentando acceder a la seccion critica\n");
                if(memzone->quiere_p == 0){
                    printf("Dentro de la Seccion Critica\n");
                    getchar();
                    printf("He salido de la Seccion Critica\n");
                    getchar();
                    memzone->quiere_q = 0;
                    printf("He accionado el pulsador\n");
                }else{
                    printf("Puerta cerrada\n");
                }  
        }
        if(letra_proceso == 'q'){
                getchar();
                memzone->quiere_p = 1;
                printf("He accionado el pulsador\n");
                getchar();
                printf("Intentando acceder a la seccion critica\n");
                if(memzone->quiere_q == 0){
                    printf("Dentro de la Seccion Critica\n");
                    getchar();
                    printf("He salido de la Seccion Critica\n");
                    getchar();
                    memzone->quiere_p = 0;
                    printf("He accionado el pulsador\n");
                }else{
                    printf("Puerta cerrada\n");
                }  
        }
        
        
        

    }

    shmdt(memzone); //Separa el espacio de memoria creado del espacio de memoria del propio proceso, al contrario que shmat

    //shmctl(id_memoria,IPC_RMID,NULL);// Libera memoria compartida creada con shmget
                                     // IPC_RMID libera la memoria cuando se haya desvinculado de un proceso con shmdt 

    return 0;
} 
  
