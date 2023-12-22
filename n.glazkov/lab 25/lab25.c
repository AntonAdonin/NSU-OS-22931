#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int main() {
    int pipefd[2];
    pid_t pid;
    
    //создание канала
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Дочерний процесс
        close(pipefd[0]);  // Закрываем конец канала для чтения

        char message[] = "Hello, World!";
        write(pipefd[1], message, sizeof(message));
        close(pipefd[1]);  // Закрываем конец канала для записи
        exit(EXIT_SUCCESS);
    } else {  // Родительский процесс
        close(pipefd[1]);  // Закрываем конец канала для записи

        char buffer[100];
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);  // Закрываем конец канала для чтения

        for (int i = 0; buffer[i]; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        printf("Результат: %s\n", buffer);

        wait(NULL);  // Ждем завершения дочернего процесса
        exit(EXIT_SUCCESS);
    }
}
