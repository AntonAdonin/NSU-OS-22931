//10. Код завершения команды
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
    
int main(int argc, char* argv[]) {
    if (argc == 1)  // если аргументов нет, то завершение программы
        return 1;

    pid_t pid = fork();  // создание подпроцесса в переменной pid
    if (pid == -1)  // если fork не удался, т.е. pid равен -1, то завершение программы
        return 1;

    if (pid == 0) {  // если fork удался, т.е. pid равен 0,
        execvp(argv[1], argv + 1);  // то execvp выполняет команду argv[1] с аргументами, начиная с argv + 1
        return 1;
    } else {  // иначе
        int stat_loc;
        wait(&stat_loc);  // ждем завершения подпроцесса

        if (WIFEXITED(stat_loc))  // если подпроцесс завершился, то выводим сообщение об этом
            printf("\nSubprocess pid: %d finished with exit code %d\n", pid, WEXITSTATUS(stat_loc));
    }

    return 0;
}
// ./lb10 printf "Hello, World!"
