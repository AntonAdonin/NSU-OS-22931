#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

struct termios saved;
int count = 0;

void sound() {
    printf("\a");
    count++;
}

void end() {
    printf("You beeped %d times\n", count);
    exit(0);
}

int main() {
    sigset(SIGINT, sound);
    sigset(SIGQUIT, end);
    while (1);
}
