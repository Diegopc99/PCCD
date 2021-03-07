#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int piso = 0;
//int piso_actual = 0;
int pid_ascensor = 0;
int senal_actual = 0;
int T_PISO = 5;

void signal_handler(int signal){

    senal_actual = signal;

}

int main(int numArgs,char *args[]){

    struct sigaction sa;
    int i = 0;

    if(numArgs != 3){
        printf("Parametros iniciales incorrectos.\n");
        return 0;
    }

    pid_ascensor = atoi(args[1]);
    piso =  atoi(args[2]);

    int pid_sensor = getpid();
    printf("PID sensor: %i\n",pid_sensor);

    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1,&sa,NULL);
    sigaction(SIGQUIT,&sa,NULL);


    while(1){
        pause();

        switch(senal_actual){

            case SIGUSR1:

                sleep(T_PISO);

                printf("Alcanzado piso %i.\n",piso);

                if(kill(pid_ascensor,SIGALRM) == -1){
                    perror("Fallo en kill.\n");
                    exit(-1);
                }
                break;

            case SIGQUIT:
                printf("Saliendo...\n");
                exit(0);

            default:
                printf("La senal no se procesa.\n");
                break;

        }
    }

    return 0;
}