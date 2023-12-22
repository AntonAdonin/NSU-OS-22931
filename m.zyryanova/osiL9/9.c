#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(int argc, char* argv[]) {
	pid_t pid = fork();
	printf("Pid process %d\n", pid);
	
	if (pid == 0) {
		printf("pid child process: %d\n", getpid());
		execlp("cat", "cat", "file.txt", NULL);
		exit(0);
	}
	else if(pid > 0) {
		printf("pid parent process: %d\n", getpid());
		wait(0);
		printf("The end\n");
	}
	else perror("child process not created\n"); 
	return 0;
}