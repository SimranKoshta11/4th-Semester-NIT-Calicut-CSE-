#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigint_handler(int sig) {
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);
    while (1) pause();
    return 0;
}

