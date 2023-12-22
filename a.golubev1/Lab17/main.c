#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <malloc.h>

int const kMaxLength = 40;

char const kKill = 0x15;
char const kBeep = 0x07;
char const kRemoveWord = 0x17;
char const kBackspace = 0x7f;
char const kClose = 0x04;

void HandlePrintable(char* line, int* position, char c) {
  line[(*position)++] = c;
  line[*position] = '\0';
  printf("%c", c);
}

void HandleWordRemoval(char* line, int* position) {
  while (*position > 0 && line[*position - 1] == ' ') {
    (*position)--;
    line[*position] = '\0';
    printf("\b \b");
  }

  while (*position > 0 && line[*position - 1] != ' ') {
    (*position)--;
    line[*position] = '\0';
    printf("\b \b");
  }
}

void HandleKill(char* line, int* position) {
  memset(line, '\0', 41);
  while (*position > 0) {
    (*position)--;
    line[*position] = '\0';
    printf("\b \b");
  }
  fflush(stdout);
}

void HandleBackspace(char* line, int* position) {
  if (*position > 0) {
    line[--(*position)] = '\0';
    printf("\b \b");
  } else {
    printf("%c", kBeep);
  }
}

void SimulateRow(void) {
  int pos = 0;
  char* row = (char*) malloc(sizeof(char) * (kMaxLength + 1));

  while (true) {
    char symbol;
    read(STDIN_FILENO, &symbol, 1);

    if (symbol == kBackspace)
      HandleBackspace(row, &pos);
    else if (symbol == kRemoveWord)
      HandleWordRemoval(row, &pos);
    else if (symbol == kKill)
      HandleKill(row, &pos);
    else if (32 <= symbol && symbol <= 126)
      HandlePrintable(row, &pos, symbol);
    else if (symbol == kClose) {
      if (pos == 0)
        break;
      else
        printf("%c", kBeep);
    }
    else
      printf("%c", kBeep);

    fflush(stdout);
  }

  free(row);
}

void GetRawInput(void) {
  struct termios new_termios;
  tcgetattr(STDIN_FILENO, &new_termios);
  new_termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void EndRawInput(void) {
  struct termios new_termios;
  tcgetattr(STDIN_FILENO, &new_termios);
  new_termios.c_lflag |= (ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

int main(void) {
  GetRawInput();

  printf("Row:\n");

  SimulateRow();

  EndRawInput();

  return 0;
}
