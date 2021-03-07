#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int piso = 0;
int num_ascensos = 0;
int num_descensos = 0;
int num_pisos = 0;
int T_PISO = 5; //Tiempo de espera en segundos

void signal_handler(int signal){

    printf("Senal %i capturada.\n",signal);

    switch(signal){
        
        case SIGUSR1: //SUBIR

            if(piso == num_pisos){
                printf("Piso final alcanzado, no se puede subir mas.\n");
            }else{
                piso++;
                num_ascensos++;
                printf("Subiendo un piso...\n");
                sleep(T_PISO);
                printf("Piso actual: %i \n",piso);
            }
            break;
        
        case SIGUSR2: //BAJAR

            if(piso == 0){
                printf("Piso numero 0 , no se puede bajar mas.\n");
            }else{
                piso--;
                num_descensos++;
                printf("Bajando un piso...\n");
                sleep(T_PISO);
                printf("Piso actual: %i \n",piso);
            }
            break;
        
        case SIGQUIT:
            printf("Numero de ascensos: %i \n",num_ascensos);
            printf("Numero de descensos: %i \n",num_descensos);
            exit(0);

        default:
            printf("La senal no se procesa.\n");
            break;
    }

}

int main(int numArgs,char *args[]){

    struct sigaction sa;
    int i = 0;

    if(numArgs != 2){
        printf("Parametros iniciales incorrectos.\n");
        return 0;
    }
    num_pisos =  atoi(args[1]);


    sa.sa_handler = signal_handler;
    //sa.sa_flags = SA_RESETHAND;
    sa.sa_flags = SA_RESTART; // Reinicia 
    //sigfillset(&sa.sa_mask); 
    
    for(i=1;i<64;i++){

        sigaction(i,&sa,NULL);
    }

    while(1){
        pause();
    }

    return 0;
}
