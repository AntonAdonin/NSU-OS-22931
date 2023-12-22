#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <ulimit.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/resource.h>
#include <limits.h>

extern char** environ;

void PrintRealAndEffectiveUserId(void) {
  printf("Real user id: %d\n", getuid());
  printf("Effective user id: %d\n", geteuid());
}

void MakeProcessGroupLeader(void) {
  setpgid(0, 0);  // If zeros, it uses current process and current process group ids
}

void PrintProcessIdentifiers(void) {
  printf("Process id: %d\n", getpid());
  printf("Group process id: %d\n", getpgrp());
  printf("Parent process id: %d\n", getppid());
}

void PrintMaxFileSize(void) {
  printf("Max file size is %ld\n", ulimit(UL_GETFSIZE));
}

bool SetMaxFileSize(long size) {
  bool is_successful = ulimit(UL_SETFSIZE, size) != -1;

  if (!is_successful)
    perror("Changing file limit failed");

  return is_successful;
}

void PrintCoreFileLimit(void) {
  struct rlimit rlimit_struct;
  getrlimit(RLIMIT_FSIZE, &rlimit_struct);
  printf("CoreFileLimit: %ld\n", rlimit_struct.rlim_cur);
}

bool SetCoreFileLimit(long limit) {
  struct rlimit rlimit_struct;
  getrlimit(RLIMIT_FSIZE, &rlimit_struct);
  rlimit_struct.rlim_cur = limit;

  bool is_successful = setrlimit(RLIMIT_FSIZE, &rlimit_struct) == 0;

  if (!is_successful)
    perror("Setting rlimit failed");

  return is_successful;
}

void PrintCurrentDirectory(void) {
  char current_directory[PATH_MAX];
  getcwd(current_directory, sizeof(current_directory));
  printf("Current directory: %s\n", current_directory);
}

void PrintEnvironmentVariables(void) {
  printf("Environment variables:\n");

  for (int i = 0; environ[i] != 0; ++i)
    printf("%d: %s\n", i, environ[i]);
}

void ChangeEnvironmentVariable(char* variable_assign_value) {
  putenv(variable_assign_value);
}

int main(int argc, char* argv[]) {
  char options[] = "ispuU:cC:dvV:";
  int option;

  while ((option = getopt(argc, argv, options)) != EOF) {
    switch (option) {
      case 'i': PrintRealAndEffectiveUserId();
        break;
      case 's': MakeProcessGroupLeader();
        break;
      case 'p': PrintProcessIdentifiers();
        break;
      case 'u': PrintMaxFileSize();
        break;
      case 'U':
        if (SetMaxFileSize(atol(optarg)) == false)
          return 1;
        break;
      case 'c': PrintCoreFileLimit();
        break;
      case 'C':
        if (SetCoreFileLimit(atol(optarg)) == false)
          return 2;
        break;
      case 'd': PrintCurrentDirectory();
        break;
      case 'v': PrintEnvironmentVariables();
        break;
      case 'V': ChangeEnvironmentVariable(optarg);
        break;
      case '?':
      default: printf("Invalid option: %c\n", optopt);
        break;
    }
  }
}
