#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

void PrintRealAndEffectiveUserId(void) {
  printf("Real user id: %d\n", getuid());
  printf("Effective user id: %d\n", geteuid());
}

bool CanOpenFile(char* path) {
  FILE* file = fopen(path, "r");
  if (file == NULL)
    return false;

  fclose(file);
  return true;
}

void SetEffectiveUserIdToReal(void) {
  setuid(geteuid());
}

int main(void) {
  char example_path[] = "example.txt";

  PrintRealAndEffectiveUserId();

  bool can_open_file = CanOpenFile(example_path);
  if (!can_open_file)
    perror("Cannot open file");

  SetEffectiveUserIdToReal();

  PrintRealAndEffectiveUserId();

  bool can_open_file_effective_id = CanOpenFile(example_path);
  if (!can_open_file_effective_id)
    perror("Cannot open file after setting id to effective");

  return 0;
}
