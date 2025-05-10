#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

sem_t tap;
sem_t pam;
sem_t mat;
sem_t ds;
int s_count[3]={0,0,0};
pthread_t st[3],at;
int stop_threads = 0;

void *smoker(void *arg){
	int id=*(int*)arg;
	while(1){
		if (stop_threads) 
			pthread_exit(NULL);
		if(id==0){
			sem_wait(&tap);
			if (stop_threads)
				pthread_exit(NULL);
			printf("Smoker with matches took tobacco and paper and then rolling and smoking\n");
		}
		else if(id==1){
			sem_wait(&pam);
			 if (stop_threads) 
			 	pthread_exit(NULL);
			printf("Smoker with tobacco took paper and matches and then rolling and smoking\n");
		}
		else{
			sem_wait(&mat);
			 if (stop_threads) 
			 	pthread_exit(NULL);
			printf("Smoker with paper took matches and tobacco and then rolling and smoking\n");
		}
		sleep(1);
		if(stop_threads)
			pthread_exit(NULL);	
		printf("smoker %d : finished smoking\n",id);
		s_count[id]++;
		
		sem_post(&ds);
	}
	return NULL;
}
void *agent(void *arg){
	while(1){
		sem_wait(&ds);
		int choice=rand()%3;
		if(choice==0){
			printf("Agent: placing tobacco and paper on the table\n");
			sem_post(&tap);
		}
		else if(choice==1){
			printf("Agent: placing paper and matches on the table\n");
			sem_post(&pam);
		}
		else{
			printf("Agent: placing matches and tobacco on the table\n");
			sem_post(&mat);
		}
		sleep(1);
		if (s_count[0] > 0 && s_count[1] > 0 && s_count[2] > 0) {
         		   printf("All smokers have smoked at least once. Terminating process.\n");
         		   stop_threads=1;
         		   for (int i = 0; i < 3; i++) {
                		sem_post(&tap);
                		sem_post(&pam);
                		sem_post(&mat);
            		   }
         		   pthread_exit(NULL);
        	}
	}
	return NULL;
}
int main(){
	//pthread_t st[3],at;
	int s_id[3]={0,1,2};
	sem_init(&tap,0,0);
	sem_init(&pam,0,0);
	sem_init(&mat,0,0);
	sem_init(&ds,0,1);
	for(int i=0; i<3; i++){
		pthread_create(&st[i],NULL,smoker,&s_id[i]);
	}
	pthread_create(&at, NULL, agent, NULL);
	pthread_join(at,NULL);
	for(int i=0; i<3; i++){
		pthread_join(st[i],NULL);
	}
	sem_destroy(&tap);
	sem_destroy(&pam);
	sem_destroy(&mat);
	sem_destroy(&ds);
	return 0;
}
