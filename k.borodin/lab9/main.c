#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } 
    if (pid > 0) {
        siginfo_t info;
        if (waitid(P_PID, pid, &info, WEXITED) == -1) {
            perror("waitid");
            exit(1);
        }
        printf("Child process pid %d exited with %d\n", info.si_pid, info.si_status);
    } else if (pid == 0) {
        system("cat input.txt");
        return 0;
    };

    return 0;
}
