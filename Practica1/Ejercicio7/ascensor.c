#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int piso = 0;
int num_ascensos = 0;
int num_descensos = 0;
//int num_pisos = 0;
//int T_PISO = 5; //Tiempo de espera en segundos
//int espera = 0;
int senal_recibida = 0;
//int pid_sensor = 0;

//void sensor_handler(int);

void signal_handler(int signal){

    senal_recibida = signal;

}

int main(int numArgs,char *args[]){

    struct sigaction sa;
    int i = 0;

    int pid_ascensor = getpid();

    printf("PID ascensor: %i\n",pid_ascensor);

    /*if(numArgs != 2){
        printf("Parametros iniciales incorrectos");
        return 0;
    }*/
    //num_pisos =  atoi(args[1]);
    //pid_sensor = atoi(args[2]);


    sa.sa_handler = signal_handler;
    //sa.sa_flags = SA_RESETHAND; //Elimina el handler despues de la primera senal
    sa.sa_flags = SA_RESTART; // Reinicia la llamada al sistema si es posible
    //sigfillset(&sa.sa_mask); //Bloquea todas las senales mientras se ejecute el handler
    
    
    for(i=1;i<64;i++){
        sigaction(i,&sa,NULL);
    }

    //Sincronizacion con el pulsador

    while(1){

        pause();

        switch(senal_recibida){
        
        case SIGUSR1: //SUBIR

            piso++;
            num_ascensos++;
            printf("Subiendo un piso...\n");

            ////////Sincronizamos con sensor

            //kill(pid_sensor,SIGALRM); //Enviamos sincronizacion al sensor

            do{
                pause();
            }while(senal_recibida != SIGALRM);

            ////////////////////////////////

            printf("Piso %i alcanzado.\n",piso);

            break;
        
        case SIGUSR2: //BAJAR

            piso--;
            num_descensos++;
            printf("Bajando un piso...\n");
            
            //Sincronizamos con sensor

            //kill(pid_sensor,SIGALRM); //Enviamos sincronizacion al sensor

            do{
                pause();
            }while(senal_recibida != SIGALRM);
            
            /////////////////////////
            printf("Piso %i alcanzado.\n",piso);

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

    return 0;
}
