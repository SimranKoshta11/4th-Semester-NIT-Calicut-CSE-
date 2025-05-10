#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAX_CMD_LEN 1024

int main(int argc, char *argv[]) {
    if (argc == 1) {
        int c_to_e[2], e_to_c[2]; 
        if (pipe(c_to_e) == -1 || pipe(e_to_c) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
        pid_t pid_C = fork();
        if (pid_C == 0) {
            close(c_to_e[0]);  
            close(e_to_c[1]);  
            char write_fd_str[10], read_fd_str[10];
            sprintf(write_fd_str, "%d", c_to_e[1]); 
            sprintf(read_fd_str, "%d", e_to_c[0]);  

            execlp("xterm", "xterm", "-T", "Command Input (C)", "-e", "./CSE", "C", write_fd_str, read_fd_str, NULL);
            perror("execlp failed for C");
            exit(EXIT_FAILURE);
        }
        pid_t pid_E = fork();
        if (pid_E == 0) {
            close(c_to_e[1]);  
            close(e_to_c[0]);  

            char read_fd_str[10], write_fd_str[10];
            sprintf(read_fd_str, "%d", c_to_e[0]); 
            sprintf(write_fd_str, "%d", e_to_c[1]); 

            execlp("xterm", "xterm", "-T", "Command Executor (E)", "-e", "./CSE", "E", read_fd_str, write_fd_str, NULL);
            perror("execlp failed for E");
            exit(EXIT_FAILURE);
        }
        close(c_to_e[0]); close(c_to_e[1]);
        close(e_to_c[0]); close(e_to_c[1]);

        waitpid(pid_C, NULL, 0);
        waitpid(pid_E, NULL, 0);

    } else if (argc == 4 && (strcmp(argv[1], "C") == 0 || strcmp(argv[1], "E") == 0)) {
        
        int current_mode = (strcmp(argv[1], "C") == 0) ? 0 : 1; 
        int write_fd, read_fd;

        if (current_mode == 0) {
            
            write_fd = atoi(argv[2]);
            read_fd = atoi(argv[3]);  
        } else {
      
            read_fd = atoi(argv[2]);  
            write_fd = atoi(argv[3]); 
        }

       
        int original_stdin = dup(STDIN_FILENO);
        int original_stdout = dup(STDOUT_FILENO);

        while (1) {
            if (current_mode == 0) { 
                char cmd[MAX_CMD_LEN];
                fprintf(stderr, "Enter Command> ");
                fflush(stderr);

                if (!fgets(cmd, MAX_CMD_LEN, stdin)) break;

                
                write(write_fd, cmd, strlen(cmd));

                if (strncmp(cmd, "exit", 4) == 0) break;
                if (strncmp(cmd, "swaprole", 8) == 0) current_mode = 1;

            } else { 
                char cmd[MAX_CMD_LEN];
                ssize_t len = read(read_fd, cmd, MAX_CMD_LEN - 1); 

                if (len <= 0) break;
                cmd[len] = '\0';

                if (strncmp(cmd, "exit", 4) == 0) break;
                if (strncmp(cmd, "swaprole", 8) == 0) {
                    current_mode = 0;
                    continue;
                }

               
                pid_t pid = fork();
                if (pid == 0) {
                    
                    dup2(original_stdin, STDIN_FILENO);
                    dup2(original_stdout, STDOUT_FILENO);
                    execlp("/bin/sh", "sh", "-c", cmd, NULL);
                    perror("execlp failed");
                    exit(EXIT_FAILURE);
                }
                waitpid(pid, NULL, 0);
            }
        }

       
        close(write_fd);
        close(read_fd);
        close(original_stdin);
        close(original_stdout);
        exit(0);
    }
    return 0;
}
