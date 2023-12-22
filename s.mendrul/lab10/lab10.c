#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	pid_t pid;
	int status;

	pid = fork();

	if(pid < 0){
		perror("Ошибка при создании процесса");
		exit(1);
	} else if(pid == 0){
		execvp(argv[1], &argv[1]);
		perror("Ошибка при запуске комманды");
		exit(1);
	} else{
		waitpid(pid, &status, 0);
		if(WIFEXITED(status)){
			printf("Код завершения: %d\n", WEXITSTATUS(status));
		} else{
			printf("Процесс завершился некорректно\n");
		}
	}
	return 0;
}
