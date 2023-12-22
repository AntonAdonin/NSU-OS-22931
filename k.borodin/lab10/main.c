#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>


int main(int argc, char *argv[]) {

    if (argc < 2){
        perror("Run without params");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } 
    if (pid > 0) {
        siginfo_t info;
        if (waitid(P_PID, pid, &info, WEXITED) == -1) {
            perror("waitid");
            exit(EXIT_FAILURE);
        }
        printf("Child process pid %d exited with %d\n", info.si_pid, info.si_status);
    } else if (pid == 0) {
        execvp(argv[1], &argv[1]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
