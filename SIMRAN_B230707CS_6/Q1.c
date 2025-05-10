#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
int shared = 0;
sem_t semaphore;
void* thread1(void* arg){
	sem_wait(&semaphore);
	printf("Thread 1 : before incrementing shared variable = %d\n",shared);
	int temp = shared;
	sleep(1);
	temp++;
	shared=temp;
	printf("Thread 1 : After incrementing shared variable = %d\n",shared);
	sem_post(&semaphore);
	return NULL;
}
void* thread2(void* arg){
	sem_wait(&semaphore);
	printf("Thread 2  : before decrementing shared variable = %d\n",shared);
	int temp = shared;
	sleep(1);
	temp--;
	shared=temp;
	printf("Thread 2 : After decrementing shared variable = %d\n",shared);
	sem_post(&semaphore);
	return NULL;
}
int main(){
	pthread_t t1,t2;
	sem_init(&semaphore,0,1);
	pthread_create(&t1,NULL,thread1,NULL);
	pthread_create(&t2,NULL,thread2,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	sem_destroy(&semaphore);
	printf("Final value of shared = %d\n",shared);
	return 0;
}
