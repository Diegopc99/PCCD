 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>


int main (){

    int i = 0;
    int pid[3] ={};
    int status = 0;
    char *texto[4] = {"Hola Mundo Proceso 1\n","Hola Mundo Proceso 2\n","Hola Mundo Proceso 3\n"};

    for(i=0;i<3;i++){

        int hijo = fork();
        //pid[i] = hijo;

        if(hijo == 0){

            //execl("p1",texto[i],(char *)NULL); // 
            printf("PID del Hijo: %d , PID del Padre: %d\n",getpid(),getppid());
            execl("p1",texto[i],(char *)NULL); // 
            sleep((i+2)*2);
            exit(i);

        }else if(hijo == -1){
            printf("Error al crear el proceso hijo.\n");
            exit(-1);
        }
    }

    for(i=0;i<3;i++){

        pid[i]=wait(&status);
        printf("Proceso hijo finalizado:\n  Codigo: %d\n  PID: %d\n",status,pid[i]);// Codigo es 256 porque codifica el status del exit y la razon por la que hizo exit.

        //WIF MACROS:

        if(WIFEXITED(status)){ //WIFEXITED: 1 si el proceso termino correctamente

            printf("  Exit status = %d\n",WEXITSTATUS(status)); // WEXITSTATUS: Si es true devuelve el codigo de finalizacion de 8 bits
        
        }

    }

    return 0;
}