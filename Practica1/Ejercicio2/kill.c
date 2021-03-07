#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    int numero_senal = atoi(argv[1]);
    pid_t pid = (pid_t) atoi(argv[2]);

    kill(pid,numero_senal);

    return 0;

}