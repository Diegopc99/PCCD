#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){

    int i = 0;

    for(i=1;i<argc;i++){
        printf("%s",argv[i]);
    }

    return 0;
}