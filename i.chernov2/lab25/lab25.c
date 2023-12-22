#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>


int main() {
    int fd[2];
    pid_t pid = 0;

    pipe(fd);
    pid = fork();

    if (pid == 0){
        char buffer[256];
        int readed_bytes = read(fd[0], buffer, sizeof(buffer));
	int i;
        for (i = 0; i< readed_bytes; i++){
            putchar(toupper(buffer[i]));
        }
    }

    else {
        char buffer[] = "text to upper";
        write(fd[1], buffer, sizeof(buffer));
    }

    close(fd[0]);
    close(fd[1]);

    return 0;
}