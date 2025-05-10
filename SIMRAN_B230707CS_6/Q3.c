#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS 5

pthread_t philosopher[NUM_PHILOSOPHERS];
pthread_mutex_t chopstick[NUM_CHOPSTICKS];

void *dine(void *arg){
	int n = *(int *)arg;
	free(arg);
	printf("Philosopher %d is thinking \n",n);
	pthread_mutex_lock(&chopstick[n]);
	pthread_mutex_lock(&chopstick[(n+1)%NUM_CHOPSTICKS]);
	printf("Philosopher %d is eating\n",n);
	sleep(3);
	pthread_mutex_unlock(&chopstick[n]);
	pthread_mutex_unlock(&chopstick[(n+1)%NUM_CHOPSTICKS]);
	printf("Philosopher %d Finished eating \n",n);
	return NULL;
}
int main(){
	int sm;
	for(int i=0; i<NUM_CHOPSTICKS; i++){
		sm=pthread_mutex_init(&chopstick[i],NULL);
		if(sm!=0){
			printf("Mutex intialization failed\n");
			exit(1);
		}
	}
	for(int i=0; i<NUM_PHILOSOPHERS; i++){
		int *arg = malloc(sizeof(*arg)); 
		*arg = i;
		sm=pthread_create(&philosopher[i], NULL, dine, arg);
		if(sm!=0){
			printf("Thread creation error\n");
			exit(1);
		}
	}
	for(int i=0; i<NUM_PHILOSOPHERS; i++){
		sm=pthread_join(philosopher[i],NULL);
		if(sm!=0){
			printf("Thread join failed\n");
			exit(1);
		}
	}
	for(int i=0; i<NUM_CHOPSTICKS; i++){
		sm=pthread_mutex_destroy(&chopstick[i]);
		if(sm!=0){
			printf("Mutex destroyed\n");
			exit(1);
		}
	}
	return 0;
}
