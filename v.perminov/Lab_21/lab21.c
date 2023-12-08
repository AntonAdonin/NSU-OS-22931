#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int count = 0;

void handleSIGINT() {
    printf("\a");
    fflush(NULL);
    count++;
}

void handleSIGQUIT() {
    printf("\nThe signal sounded %d times.\n", count);
    exit(0);
}

int main() {
    while (1) {
        signal(SIGINT, &handleSIGINT);
        signal(SIGQUIT, &handleSIGQUIT);
    }

    while (1);
}