#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>


int main (){

    int pid_Carvajal =0,pid_Ramos=0,pid_Pique=0,pid_JordiAlba=0,pid_Thiago=0;//pid_Busquets=0,pid_Silva=0,pid_Isco=0,pid_Aspas = 0;
    int status, pid_espera = 0;
    int contador=0;
    int silva_listo=0;//aspas_listo=0,busquets_listo=0;

    printf("De Gea      ");
    //fflush(stdout); //Si De Gea sale mal anadimos el fflush

    switch((pid_Carvajal=fork())){

        case 0:
            //printf("Carbajal    ");
            if(execl("p2","Carvajal","Carvajal   ",(char *) NULL) == -1){
                exit(-1);
            }
            exit(0);
        case -1:
            printf("Error en el fork de Carvajal.\n");
            exit(-1);
    }

    switch((pid_Ramos=fork())){

        case 0:
            //printf("Ramos     ");
            if(execl("p2","Ramos","Ramos   ",(char *) NULL) == -1){
                exit(-1);
            }
            exit(0);
        case -1:
            printf("Error en el fork de Ramos.\n");
            exit(-1);
    }

    switch((pid_Pique=fork())){

        case 0:
            //printf("Pique     ");
            if(execl("p2","Pique","Pique   ",(char *) NULL) == -1){
                exit(-1);
            }
            exit(0);
        case -1:
            printf("Error en el fork de Pique.\n");
            exit(-1);
    }

    switch((pid_JordiAlba=fork())){

        case 0:
            //printf("Jordi Alba      ");
            if(execl("p2","JordiAlba","Jordi Alba   ",(char *) NULL) == -1){
                exit(-1);
            }
            exit(0);
        case -1:
            printf("Error en el fork de Jordi Alba.\n");
            exit(-1);
    }

    while(contador!=4){

        pid_espera = wait(&status);

        if(pid_espera == pid_JordiAlba){
            //jordialba_listo = 1;
            contador++;
            switch(pid_Thiago = fork()){
                case 0:
                    //printf("Thiago     ");
                    //printf("Silva   ");
                    if(execl("p2","Thiago","Thiago     Silva   ",(char *) NULL) == -1){
                        exit(-1);
                    }
                    exit(0);
                case -1:
                    printf("Error en el fork de Thiago.\n");
                    exit(-1);
            }
        }

        if(pid_espera == pid_Thiago){
            silva_listo = 1;
        }

        if(pid_espera == pid_Carvajal){
            //carvajal_listo = 1;
            contador++;
        }
        if(pid_espera == pid_Ramos){
            //ramos_listo = 1;
            contador++;
        }
        if(pid_espera == pid_Pique){
            //pique_listo = 1;
            contador++;
        }
    }

    //if(carvajal_listo ==1 && ramos_listo ==1 && pique_listo==1 && jordialba_listo==1){
        /*switch(pid_Busquets = fork()){
            case 0:
                printf("Busquets  ");
                exit(0);
            case -1:
                printf("Error en el fork de Busquets.\n");
                exit(-1);
        }*/
        printf("Busquets    ");
        printf("Isco    ");
        printf("Aspas   ");
    //}
    //contador = 0;

    if(silva_listo != 1){
        pid_espera = wait(&status);
        silva_listo =1;
    }

    if(/*aspas_listo==1) && (busquets_listo ==1) &&*/ silva_listo==1){
        printf("Morata\n");
    }

    /*while(contador ==0){
        pid_espera = wait(&status); //Esperamos por busquets

        if(pid_espera == pid_Busquets){
            busquets_listo =1;
            contador =1;
            switch(pid_Isco = fork()){
                case 0:
                    printf("Isco  ");
                    printf("Aspas   ");
                    exit(0);
                case -1:
                    printf("Error en el fork de Isco.\n");
                    exit(-1);
            }
        }
    }

    contador = 0;

    while(contador ==0){

        pid_espera = wait(&status); //Esperamos por Isco

        if(pid_espera == pid_Isco){
            contador =1;
            aspas_listo =1;
        }
    }

    if(silva_listo != 1){
        pid_espera = wait(&status);
        silva_listo =1;
    }
   
    if((aspas_listo==1) && (busquets_listo ==1) && (silva_listo==1)){
        printf("Morata\n");
    }*/
    
    return 0;
} 
