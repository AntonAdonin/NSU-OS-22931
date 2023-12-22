#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int sigint_count = 0;

void sigint_handler(int signo) {
    if (signo == SIGINT) {
        sigint_count++;
        printf("\a"); // Издать звуковой сигнал на терминале
    }
}

// Обработчик сигнала SIGQUIT
void sigquit_handler(int signo) {
    if (signo == SIGQUIT) {
        printf("\nSIGQUIT. Total signals: %d\n", sigint_count);
        exit(EXIT_SUCCESS);
    }
}

int main() {
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
        exit(EXIT_FAILURE);
    
    if (signal(SIGQUIT, sigquit_handler) == SIG_ERR)
        exit(EXIT_FAILURE);

    while (1) 
        sleep(1); 

    return 0;
}