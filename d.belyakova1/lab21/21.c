#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int count = 0;

void sigint_helper(int signo) {

    if (signo == SIGINT) 
    {
        count++;
        printf("\a");
    }
}

void sigquit_helper(int signo) 
{
    if (signo == SIGQUIT) 
    {
        printf("\nSIGQUIT. Total: %d\n", count);
        exit(EXIT_SUCCESS);
    }
}

int main() 
{
    if (signal(SIGINT, sigint_helper) == SIG_ERR) 
    {
        perror("Error SIGINT");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGQUIT, sigquit_helper) == SIG_ERR) 
    {
        perror("Error SIGQUIT");
        exit(EXIT_FAILURE);
    }

    while (1) 
    {
        sleep(1);
    }

    return 0;
}