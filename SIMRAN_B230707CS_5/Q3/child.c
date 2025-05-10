#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int idx;
int status = 0;  

void sigusr1_handler(int sig) {
    if (status == 3) return;  
    double r = (double)rand() / RAND_MAX;
    if (r <0.8) {
        printf("Child %d: CATCH\n", idx);
        kill(getppid(), SIGUSR1);
    } else {
        printf("Child %d: MISS\n", idx);
        status = 2;
        kill(getppid(), SIGUSR2);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <index>\n", argv[0]);
        return 1;
    }
    
    idx = atoi(argv[1]);
    srand(time(NULL) ^ getpid());

    signal(SIGUSR1, sigusr1_handler);

    while (1) pause();  
}

