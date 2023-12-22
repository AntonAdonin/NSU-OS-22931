#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile int count = 0;

void sigint_handler(int sig) {
    printf("Ding!\n");
    count++;
}

void sigquit_handler(int sig) {
    printf("\nTotal ding count: %d\n", count);
    _exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, sigquit_handler);

    while (1) {
        // Бесконечный цикл
    }

    return 0;
}
