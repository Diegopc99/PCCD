#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int senal_recibida = 0;
int LOOP = 0;

void signal_handler(int signal){

    senal_recibida = signal;

}

int main (){

    int i = 0;
    
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD,&sa,NULL);

    for(i=0;i<3;i++){

        int hijo = fork();


        if(hijo == 0){

            printf("PID del Hijo: %d , PID del Padre: %d\n",getpid(),getppid());
            sleep((i+2)*2);
            kill(getpid(),SIGCHLD);

            return 0;

        }else if(hijo == -1){
            printf("Error al crear el proceso hijo.\n");
            return -1;
        }
    }

    while(LOOP < 3){

        pause();

        switch(senal_recibida){

            case SIGCHLD:
                LOOP++;
                printf("Proceso hijo finalizado.\n");
                break;
            default:
                break;

        }
    }

    return 0;
}
