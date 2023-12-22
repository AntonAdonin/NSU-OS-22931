#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int count;

int main()
{
           void sigf(int);
           signal(SIGINT, sigf);
           signal(SIGQUIT, sigf);
           while (1);
    return 0;
}

void sigf(int sig)
{
    if (sig == SIGQUIT) {
        printf("Signal has rung for %d times\n", count);
        exit(1);
    }
    printf("%c\n", '\07');
    count++;
    signal(sig, sigf);
}