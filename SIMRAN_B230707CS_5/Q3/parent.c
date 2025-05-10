#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_CHILDREN 10

int status[MAX_CHILDREN]; 
int n, playing;
pid_t children[MAX_CHILDREN];
int last_throw = -1;

void sigusr2_handler(int sig) {
    for (int i = 0; i < n; i++) {
        if (children[i] == last_throw && status[i] == 0) {  
            status[i] = 3; 
            playing--;  
            break;
        }
    }
}

void sigchld_handler(int sig) {
    int child_status;
    pid_t pid;

    while ((pid = waitpid(-1, &child_status, WNOHANG)) > 0) { 
        for (int i = 0; i < n; i++) {
            if (children[i] == pid) {
                status[i] = 3; 
                playing--;
                break;
            }
        }
    }
}

void print_status() {
    printf("\nCurrent Status:\n");
    for (int i = 0; i < n; i++) {
        if (status[i] == 0) printf("Child %d: PLAYING\n", i);
        else if (status[i] == 2) printf("Child %d: MISSED\n", i);
        else if (status[i] == 3) printf("Child %d: OUT\n", i);
    }
}

int next_child() {
    int next;
    int count = 0;
    
    do {
        next = rand() % n;  
        count++;
        if (count >= n) return -1;  
    } while (status[next] == 3);

    return next;
}

void sigusr1_handler(int sig) {
    printf("Received SIGUSR1 in parent process\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_children>\n", argv[0]);
        return 1;
    }
    
    n = atoi(argv[1]);
    if (n > MAX_CHILDREN) {
        printf("Error: Maximum children allowed is %d\n", MAX_CHILDREN);
        return 1;
    }
    playing = n;

    signal(SIGUSR2, sigusr2_handler);
    signal(SIGCHLD, sigchld_handler);
    signal(SIGUSR1, sigusr1_handler);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            char idx[5];
            sprintf(idx, "%d", i);
            execl("./child", "child", idx, NULL);
            perror("execl failed");
            exit(1);
        }
        children[i] = pid;
        status[i] = 0;
    }

    sleep(1);

    while (playing > 1) {
        int next = next_child();
        if (next == -1) break; 

        last_throw = children[next];
        printf("\nParent throwing ball to Child %d\n", next);
        kill(children[next], SIGUSR1);
        print_status();
        
        sleep(1);
    }

  
    for (int i = 0; i < n; i++) {
        if (status[i] != 3) {
            printf("\nGame over! Winner: Child %d\n", i);
            break;
        }
    }

    return 0;
}

