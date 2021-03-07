#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){

    int pausa = 0;

    pausa = pause();

    if(pausa ==-1){
        printf("Error al pausar el proceso.\n");
    }

    return 0;

}