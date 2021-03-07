#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int num_SIGUSR1 = 0;
int num_SIGUSR2 = 0;

void signal_handler(int signal){

    printf("Senal %i capturada.\n",signal);

    switch(signal){
        
        case SIGUSR1:
            num_SIGUSR1++;
            printf("Capturada SIGUSR1.\n");
            break;
        
        case SIGUSR2:
            num_SIGUSR2++;
            printf("Capturada SIGUSR2.\n");
            break;

        case SIGTERM:
            printf("Capturada SIGTERM.\n");
            printf("Numero de senales SIGUSR1: %i\n",num_SIGUSR1);
            printf("Numero de senales SIGUSR2: %i\n",num_SIGUSR2);
            printf("Saliendo....\n");
            exit(0);

        default:
            printf("La senal no se procesa.\n");
            break;
    }

}

int main(){

    struct sigaction sa;
    int i = 0;

    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESETHAND;
    //sigemptyset(&sa.sa_mask);//Anade a la mascara todas las senales
    
    if(sigaction(SIGUSR1,&sa,NULL) == -1){
        perror("Error en sigaction.\n");
    }

    if(sigaction(SIGUSR2,&sa,NULL) == -1){
        perror("Error en sigaction.\n");
    }

    if(sigaction(SIGTERM,&sa,NULL) == -1){
        perror("Error en sigaction.\n");
    }

    /*for(i=1;i<64;i++){
        
        if(i == SIGUSR1 || i == SIGUSR2 || i == SIGTERM){
            sa.sa_handler = signal_handler;
            sa.sa_flags = SA_RESTART;
            sigaction(i,&sa,NULL);
            continue;
        }
        
        sa.sa_handler = SIG_IGN;
        sigaction(i,&sa,NULL);
        
    }*/

    while(1){
        pause();
    }

    return 0;
}