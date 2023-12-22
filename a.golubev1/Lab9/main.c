#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(void) {
  pid_t pid = fork();

  if (pid < 0) {
    printf("\nERROR: Unable to fork. Abort");
    return 1;
  }

  bool is_child = pid == 0;
  if (is_child) {
    char* args[] = {"cat", "input.txt", NULL};

    int result = execvp("cat", args);

    return result;
  }

  int status;
  waitpid(pid, &status, 0);

  printf("\nChild status: %d\n", WEXITSTATUS(status));

  return 0;
}
