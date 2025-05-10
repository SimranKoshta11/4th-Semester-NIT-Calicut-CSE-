#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
int warehouse[BUFFER_SIZE];
int count=0;
sem_t empty_slots;
sem_t full_slots;
pthread_mutex_t mutex;

void* producer(void *arg){
	for(int i=1; i<=10; i++){
		sem_wait(&empty_slots);
		pthread_mutex_lock(&mutex);
		warehouse[count]=i;
		printf("Producer added item %d and the total items %d\n",i,++count);
		pthread_mutex_unlock(&mutex);
		sem_post(&full_slots);
		sleep(1);
	}
	return NULL;
}
void* consumer(void *arg){
	for(int i=1; i<=10; i++){
		sem_wait(&full_slots);
		pthread_mutex_lock(&mutex);
		int item = warehouse[--count];
		printf("Consumer took item %d and the remaining items %d\n",i,count);
		pthread_mutex_unlock(&mutex);
		sem_post(&empty_slots);
		sleep(2);
	}
	return NULL;
}
int main(){
	pthread_t producer_thread,consumer_thread;
	sem_init(&empty_slots,0,BUFFER_SIZE);
	sem_init(&full_slots,0,0);
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&producer_thread,NULL,producer,NULL);
	pthread_create(&consumer_thread,NULL,consumer,NULL);
	pthread_join(producer_thread,NULL);
	pthread_join(consumer_thread,NULL);
	sem_destroy(&empty_slots);
	sem_destroy(&full_slots);
	pthread_mutex_destroy(&mutex);
	printf("Final Count value is : %d\n",count);
	return 0;
}
