#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int piso_actual = 0;
int piso_maximo = 0;

int main(int numArgs, char *args[]){

    int j = 0;
    int pid_ascensor = atoi(args[1]);
    piso_maximo = atoi(args[2]) -1; //Porque contamos el piso 0

    int pid_sensor[15] = {};
    
    //printf("Introduce pid sensores: \n");

    for(j=0;j<=piso_maximo;j++){
        printf("Introduzca el pid del sensor %i: ",j);
        scanf("%i",&pid_sensor[j]);

    }

    int seleccion;

    printf("MENU DE SELECCION:\n");
    printf("1) SUBIR\n");
    printf("2) BAJAR\n");
    printf("3) SALIR\n");

    while(1){

        printf("Introduzca accion: ");
        scanf("%i",&seleccion);

        switch(seleccion){
        
            case 1:

                if(piso_actual == piso_maximo){

                    printf("Piso maximo alcanzado, no se puede subir mas.\n");
                    
                }else{

                    piso_actual++;

                    if(kill(pid_ascensor,SIGUSR1) == -1){
                        perror("Fallo en kill.\n");
                        exit(-1);
                    }
                    if(kill(pid_sensor[piso_actual],SIGUSR1) == -1){ //Sensor del siguiente piso
                        perror("Fallo en kill.\n");
                        exit(-1);
                    }
                    //piso_actual++;
                }
            break;

            case 2:
                if(piso_actual == 0){

                    printf("Piso mas bajo alcanzado, no se puede bajar mas.\n");
                
                }else{

                    piso_actual--;

                    if(kill(pid_ascensor,SIGUSR2) == -1){
                        perror("Fallo en kill.\n");
                        exit(-1);
                    }
                    if(kill(pid_sensor[piso_actual],SIGUSR1) == -1){
                        perror("Fallo en kill.\n");
                        exit(-1);
                    }
                    
                }
            break;

            case 3:
                if(kill(pid_ascensor,SIGQUIT) == -1){
                    perror("Fallo en kill.\n");
                    exit(-1);
                }
                for(j=0;j<=piso_maximo;j++){
                    if(kill(pid_sensor[j],SIGQUIT) == -1){
                        perror("Fallo en kill.\n");
                        exit(-1);
                    }
                }
                printf("Saliendo...\n");
                exit(0);
            
            default:
                printf("Accion no soportada.\n");
            break;

        }
    }
}