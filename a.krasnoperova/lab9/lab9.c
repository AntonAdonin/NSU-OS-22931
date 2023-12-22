#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main()
{
     pid_t pid;
     int status;
     if (!(pid = fork())) execl("cat", "meow.txt", (char*)0);
     printf("Waiting for child: %ld\n", pid);
     pid_t ret = wait(&status);
     printf("Wait's return value of parent: %ld,", ret);
     printf("Status of child: %d\n", WEXITSTATUS(status));
     return 0;
}