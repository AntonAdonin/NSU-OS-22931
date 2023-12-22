#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(int argc, char** argv) {
  if (argc < 2)
  {
    fprintf(stderr, "ERROR: Not enough arguments. Abort");
    return 1;
  }

  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "ERROR: Unable to fork. Abort");
    return 2;
  }

  bool is_child = pid == 0;
  if (is_child) {
    int result = execvp(argv[1], argv + 1);
    return result;
  }

  int status;
  waitpid(pid, &status, 0);

  printf("\nChild status: %d\n", WEXITSTATUS(status));

  return 0;
}
