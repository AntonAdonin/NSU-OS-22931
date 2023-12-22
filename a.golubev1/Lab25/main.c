#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/wait.h>

const int kStringLength = 256;

int ChildProcessBody(int fd[2]) {
  close(fd[1]);

  char string[kStringLength];

  for (int i = 0; read(fd[0], string, kStringLength) > 0; ++i) {

    for (int j = 0; string[j] != 0; ++j)
      string[j] = (char) toupper(string[j]);

    printf("%s\n", string);
  }

  close(fd[0]);

  return 0;
}

int main() {
  const int kStringsCount = 3;
  char strings[3][256] = {"Amogus", "Bebra", "Sufler"};

  int fd[2];
  if (pipe(fd) == -1) {
    fprintf(stderr, "Pipe failed");
    return 1;
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed");
    return 2;
  }

  bool is_child = pid == 0;
  if (is_child)
    return ChildProcessBody(fd);

  close(fd[0]);

  for (int i = 0; i < kStringsCount; ++i)
    write(fd[1], strings[i], kStringLength);

  close(fd[1]);

  int status;
  waitpid(pid, &status, 0);

  return 0;
}
