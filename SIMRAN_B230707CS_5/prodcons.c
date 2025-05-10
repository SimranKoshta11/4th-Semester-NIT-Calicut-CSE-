#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

#define SHM_KEY 1234

volatile sig_atomic_t terminate_flag = 0;

void handle_signal(int sig){
	terminate_flag=1;
}

void producer(int n, int t, int *M) {
    int *counts = calloc(n + 1, sizeof(int));
    int *sums = calloc(n + 1, sizeof(int));
    srand(time(NULL));

    printf("\nProducer is alive\n\n");

    for (int i = 0; i < t && !terminate_flag; i++) {
        int item = rand() % 900 + 100;
        int consumer = rand() % n + 1;
        while (M[0] != 0 && !terminate_flag);
        
        if(terminate_flag) break;
        
        M[0] = consumer;
        #ifdef SLEEP
        usleep(rand()%10+1);
        #endif
        M[1] = item;
    
        #ifdef VERBOSE
        printf("Producer produces %d for Consumer %d\n", item, consumer);
        #endif
        
        counts[consumer]++;
        sums[consumer] += item;
    }
    
    while (M[0] != 0 && !terminate_flag);
    
    if(!terminate_flag) 
    	M[0] = -1;
    	
    for(int i=0; i<n; i++){
    	wait(NULL);
    }

    printf("\nProducer has produced %d items\n\n", t);
    for (int i = 1; i <= n; i++) {
        printf("%d items for Consumer %d: Checksum = %d\n", counts[i], i, sums[i]);
    }

    free(counts);
    free(sums);
}

void consumer(int id, int *M) {
    int count = 0, sum = 0;

    printf("Consumer %d is alive\n", id);

    while (!terminate_flag) {
        while (M[0] != id && M[0] != -1 && !terminate_flag);

        if (M[0] == -1 || terminate_flag) break;
        
        int item = M[1];

        count++;
        sum += item;

        #ifdef VERBOSE
        printf("Consumer %d reads %d\n", id, item);
        #endif
        M[0]=0;
    }

    printf("Consumer %d has read %d items: Checksum = %d\n", id, count, sum);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_consumers> <num_items>\n", argv[0]);
        exit(1);
    }
    signal(SIGINT,handle_signal);
    int n = atoi(argv[1]); 
    int t = atoi(argv[2]); 
    
    int shmid = shmget(SHM_KEY, 2 * sizeof(int), IPC_CREAT | 0666);
    if(shmid==-1){
    	perror("shmget");
    	exit(1);
    }
    int *M = (int *)shmat(shmid, NULL, 0);
    if(M==(int*)-1){
    	perror("shmat");
    	exit(1);
    }
    M[0] = 0;

    for (int i = 0; i < n; i++) {
        if (fork() == 0) {
            consumer(i + 1, M);
        }
    }
    
    producer(n, t, M);
    
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    
    shmdt(M);
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}
