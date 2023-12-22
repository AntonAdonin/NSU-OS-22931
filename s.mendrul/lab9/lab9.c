#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	pid_t pid;

	pid = fork();

	if(pid < 0){
		fprintf(stderr, "Ошибка при создании процесса\n");
		exit(1);
	} else if(pid == 0){
		//Код, выполняемый в подпроцессе
		execlp("cat", "cat", "long_file.txt", NULL);
		fprintf(stderr, "Ошибка при выполнении команды cat\n");
		exit(1);
	} else{
		//Код, выполняемый в родительском процессе
		printf("Это родительский процесс\n");
		printf("Кря\n");

		int status;
		waitpid(pid, &status, 0);
		if(WIFEXITED(status)){
			printf("Подпроцесс успешно завершился\n");
		} else{
			printf("Подпроцесс завершился с ошибкой\n");
		}
	}

	return 0;
}
