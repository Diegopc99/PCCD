#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int A[64] = {0}; //Incializamos el array con 0s

void signal_handler(int sig){

    int i =0;
    printf("Senal SIGTERM numero %i capturada.\n",sig);

    A[sig] = 1;

    if(sig == 15){

        printf("Vector de bits: \n");
        for(i=0;i<64;i++){
            printf("%i ",A[i]);
        }
        printf("\n");

        printf("SIGTERM recibida, saliendo...\n");
        exit(0);
    }

}

int main(){

    int i = 0;
    
    struct sigaction act;

    act.sa_handler = signal_handler;

    for(i=1;i<64;i++){
        sigaction(i,&act,NULL);
    }
    while(1){

        pause();

    }

    return 0;
}