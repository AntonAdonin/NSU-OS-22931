#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 256

int main() 
{
    int pipe_fd[2];
    pid_t child_pid;

    if (pipe(pipe_fd) == -1) 
    {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    if ((child_pid = fork()) == -1) 
    {
        perror("Error creating a baby process");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) 
    {
        close(pipe_fd[1]);

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE)) > 0) 
        {
            for (ssize_t i = 0; i < bytes_read; i++) 
            {
                buffer[i] = toupper(buffer[i]);
            }

            write(STDOUT_FILENO, buffer, bytes_read);
        }

        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    }

    else 
    {
        close(pipe_fd[0]);

        const char *message = "Heyyo!\n";

        write(pipe_fd[1], message, strlen(message));

        close(pipe_fd[1]);

        waitpid(child_pid, NULL, 0);
    }

    return 0;
}