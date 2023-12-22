#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();
    if (pid == -1) return 0;

    if (pid == 0) {
        // Child process
        execlp("cat", "cat", "testing.txt", NULL);
        return 1;
    } else {
        // Parent process
        if(wait(NULL) != -1) {
            printf("\nChild process (pid: %d) finished\n", pid);
        }
    }

    return 0;
}