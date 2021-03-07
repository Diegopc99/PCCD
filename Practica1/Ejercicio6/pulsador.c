#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


int main(int numArgs, char *args[]){

    int pid = atoi(args[1]);
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
                if(kill(pid,SIGUSR1) == -1){
                    perror("Fallo en kill.\n");
                    exit(-1);
                }
            break;

            case 2:
                if(kill(pid,SIGUSR2) == -1){
                    perror("Fallo en kill.\n");
                    exit(-1);
                }
            break;

            case 3:
                if(kill(pid,SIGQUIT) == -1){
                    perror("Fallo en kill.\n");
                    exit(-1);
                }
                printf("Saliendo...\n");
                exit(0);
            
            default:
                printf("Accion no soportada.\n");
            break;

        }
    }
}