#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t subprocess = fork();
    switch (subprocess) {
    case -1:
        perror("Not created");
        exit(1);
    case 0:
        printf("Subrocess starts working here\n");
        execlp("cat", "cat", "test9.txt", NULL);
        exit(0);
    default:
        printf("Process starts work here\n");
        printf("Subprocess PID is %d\n", subprocess);
        wait(0);
        printf("\nSubprocess finish work\n");
    }
    return 0;
}