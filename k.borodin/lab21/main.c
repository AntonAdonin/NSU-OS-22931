#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define SIGOUT 'q'
#define SIGACT 'k'

int cycle();

void sigintHandler() {
    putchar('\a');
    fflush(stdout);
}

int main() {
    cycle();

    return 0;
}

int cycle() {
    system("stty -icanon");
    signal(SIGINT, sigintHandler);

    int c = 0;
    while (1) {
        char ch = getc(stdin);
        if (ch == SIGOUT) return c;
        if (ch == SIGACT) raise(SIGINT), c++;
    }
}