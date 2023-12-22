#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int count = 0;

void sigint_handler() {
    printf("\a");
    printf("SIGINT\n");
    count++;
}

void sigquit_handler() {
    printf("SIGQUIT %d times\n", count);
    exit(0);
}

int main() {
    signal(SIGINT, &sigint_handler);
    signal(SIGQUIT, &sigquit_handler);

    while (1) {
        // Бесконечный цикл
    }

    return 0;
}