#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ATMS 3
#define INITIAL_BALANCE 1000

pthread_mutex_t lock;
int account_balance=INITIAL_BALANCE;
void* atm_transaction(void* arg){
	int atm_id=*(int*)arg;
	int choice,amount;
	choice = rand() % 2 + 1;
	amount=(rand()%500)+100;
	pthread_mutex_lock(&lock);
	printf("ATM %d checking account balance \n",atm_id);
	printf("ATM %d current balance : Rs.%d\n",atm_id,account_balance);
	if(choice==1){
		if(account_balance>=amount){
			account_balance-=amount;
			printf("ATM %d withdraw Rs.%d. New balance Rs.%d\n",atm_id,amount,account_balance);
		}
		else{
			printf("ATM %d Insufficient balance!\n",atm_id);
		}
	}
	else{
		account_balance+=amount;
		printf("ATM %d Deposite Rs.%d. New balance %d\n",atm_id,amount,account_balance);
	}
	pthread_mutex_unlock(&lock);
	return NULL;
}
int main(){
	srand(time(NULL)); 
	pthread_t atms[NUM_ATMS];
	int atm_ids[NUM_ATMS]={1,2,3};
	pthread_mutex_init(&lock,NULL);
	for(int i=0; i<NUM_ATMS; i++){
		pthread_create(&atms[i],NULL,atm_transaction,&atm_ids[i]);
	}
	for(int i=0; i<NUM_ATMS; i++){
		pthread_join(atms[i],NULL);
	}
	pthread_mutex_destroy(&lock);
	printf("All transactions are complete.Final balance: Rs.%d\n",account_balance);
	return 0;
}
