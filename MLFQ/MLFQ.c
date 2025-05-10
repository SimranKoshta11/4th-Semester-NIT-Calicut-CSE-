#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define MAX_JOBS 256
#define TIME_SLICE 5

typedef struct {
    char name;
    int total_time;
    int remaining_time;
    int turnaround_time;
} Job;

Job* jobs;  
int num_jobs;  
pthread_mutex_t mutex;

void* round_robin(void *arg) {
    printf("Round Robin\n");
    pthread_mutex_lock(&mutex);

    int* current_time = (int*)arg;

    for (int i = 0; i < num_jobs; i++) {
        if (jobs[i].remaining_time > 0) {
            int execution_time = (jobs[i].remaining_time > TIME_SLICE) ? TIME_SLICE : jobs[i].remaining_time;
            jobs[i].remaining_time -= execution_time;
            *current_time += execution_time;

            if (jobs[i].remaining_time == 0 && jobs[i].turnaround_time == 0) {
                jobs[i].turnaround_time = *current_time;
            }

            printf("Job %c executed for %d ms, remaining: %d ms\n",
                   jobs[i].name, execution_time, jobs[i].remaining_time);
        }
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* first_come_first_serve(void* arg) {
    printf("First Come First Serve\n");
    pthread_mutex_lock(&mutex);

    int* current_time = (int*)arg;

    for (int i = 0; i < num_jobs; i++) {
        if (jobs[i].remaining_time > 0) {
            int execution_time = (jobs[i].remaining_time > TIME_SLICE) ? TIME_SLICE : jobs[i].remaining_time;
            jobs[i].remaining_time -= execution_time;
            *current_time += execution_time;

            if (jobs[i].remaining_time == 0 && jobs[i].turnaround_time == 0) {
                jobs[i].turnaround_time = *current_time;
            }

            printf("Job %c executed for %d ms, remaining: %d ms\n",
                   jobs[i].name, execution_time, jobs[i].remaining_time);
        }
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* priority_queue(void* arg) {
    printf("Priority Queue\n");
    pthread_mutex_lock(&mutex);

    int* current_time = (int*)arg;

   
    for (int i = 0; i < num_jobs - 1; i++) {
        for (int j = 0; j < num_jobs - 1 - i; j++) {
            if (jobs[j].remaining_time > jobs[j + 1].remaining_time) {
                Job temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < num_jobs; i++) {
        if (jobs[i].remaining_time > 0) {
            int execution_time = (jobs[i].remaining_time > TIME_SLICE) ? TIME_SLICE : jobs[i].remaining_time;
            jobs[i].remaining_time -= execution_time;
            *current_time += execution_time;

            if (jobs[i].remaining_time == 0 && jobs[i].turnaround_time == 0) {
                jobs[i].turnaround_time = *current_time;
            }

            printf("Job %c executed for %d ms, remaining: %d ms\n",
                   jobs[i].name, execution_time, jobs[i].remaining_time);
        }
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* shortest_job_first(void* arg) {
    printf("Shortest Job First\n");
    pthread_mutex_lock(&mutex);

    int* current_time = (int*)arg;

    for (int i = 0; i < num_jobs - 1; i++) {
        for (int j = 0; j < num_jobs - 1 - i; j++) {
            if (jobs[j].remaining_time > jobs[j + 1].remaining_time) {
                Job temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < num_jobs; i++) {
        if (jobs[i].remaining_time > 0) {
            int execution_time = (jobs[i].remaining_time > TIME_SLICE) ? TIME_SLICE : jobs[i].remaining_time;
            jobs[i].remaining_time -= execution_time;
            *current_time += execution_time;

            if (jobs[i].remaining_time == 0 && jobs[i].turnaround_time == 0) {
                jobs[i].turnaround_time = *current_time;
            }

            printf("Job %c executed for %d ms, remaining: %d ms\n",
                   jobs[i].name, execution_time, jobs[i].remaining_time);
        }
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void MLFQ() {
    int done;
    int current_time = 0;  

    do {
        done = 1;

        pthread_t queue0, queue1, queue2, queue3;

        pthread_create(&queue0, NULL, round_robin, &current_time);
        pthread_join(queue0, NULL);

        pthread_create(&queue1, NULL, first_come_first_serve, &current_time);
        pthread_join(queue1, NULL);

        pthread_create(&queue2, NULL, priority_queue, &current_time);
        pthread_join(queue2, NULL);

        pthread_create(&queue3, NULL, shortest_job_first, &current_time);
        pthread_join(queue3, NULL);

        for (int i = 0; i < num_jobs; i++) {
            if (jobs[i].remaining_time > 0)
                done = 0;
        }

        if (!done) {
            printf("Moving back to queue0....\n");
        }
    } while (!done);
  
    printf("\nJobs Turnaround Times:\n");
    float total_turnaround_time = 0.0;
    for(int i=0; i<num_jobs; i++){
    	for(int j=0; j<num_jobs-i-1; j++){
    		if(jobs[j].name-'A'>jobs[j+1].name-'A'){
    			Job temp = jobs[j];
                	jobs[j] = jobs[j + 1];
                	jobs[j + 1] = temp;
                }
         }
    }
    for (int i = 0; i < num_jobs; i++) {
        printf("Job %c Turnaround Time: %d ms\n", jobs[i].name, jobs[i].turnaround_time);
        total_turnaround_time += jobs[i].turnaround_time;
    }
    printf("Average Turnaround Time: %.2f ms\n", total_turnaround_time / num_jobs);
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    printf("Enter the number of jobs (max 256): ");
    scanf("%d", &num_jobs);

    if (num_jobs <= 0 || num_jobs > MAX_JOBS) {
        printf("Invalid number of jobs. Must be between 1 and 256.\n");
        return 1;
    }

    jobs = (Job*)malloc(num_jobs * sizeof(Job));
    if (jobs == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    for (int i = 0; i < num_jobs; i++) {
        jobs[i].name = 'A' + i;
        printf("For process %c Total CPU time needed: ", jobs[i].name);
        scanf("%d", &jobs[i].total_time);
        jobs[i].remaining_time = jobs[i].total_time;
        jobs[i].turnaround_time = 0;
    }

    MLFQ();

    free(jobs);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

