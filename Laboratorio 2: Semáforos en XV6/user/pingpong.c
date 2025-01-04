#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
printPing (int n)
{
    for (int i = n; i > 0; i--){
        printf("ping\n");
        sem_up(1);
        sem_down(0);
    }

    sem_up(1);
}

void 
printPong (int n)
{
    sem_down(1);

    for (int i = n; i > 0; i--){
        printf("    pong\n");
        sem_up(0);
        sem_down(1);
    }
}

void 
main (int argc, char *argv[])
{
    int n;

    if (argc == 1){
        printf("Se esperan más argumentos\n");
        exit(1);
    }    
    n = atoi(argv[1]);

    semaphores_disabled(); // 

    sem_open(0,1);
    sem_open(1,1);
    int pid = fork();

    if(pid == 0){
        printPing(n);
    } else{
        printPong(n);
        sem_close(0);
        sem_close(1);        
    }

    exit(0);
}