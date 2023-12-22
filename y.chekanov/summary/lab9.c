// 9. Создание двух процессов
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();  // создание подпроцесса
    if (pid == -1)  // если fork не удался, возвращает -1
        return 1;
    if (pid == 0) {  // если процесс имеет индентификатор 0, то выполняется cat для task.c
        execlp("cat", "cat", "lab9.c", NULL);  // третий аргумент - то, что надо распечатать
        return 1;
    }
    else {  // иначе, если процесс не является родительским, то он ждет окончание подроцесса
        if (wait(NULL) != -1)  // с помощью wait
            printf("\nSubprocess pid: %d finished\n", pid);
    }

    return 0;
}
