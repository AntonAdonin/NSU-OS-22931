#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int counter = 0;

void Prompter(int signal){
  printf("\a");
  ++counter;
}

void Report(int signal){
  printf("\n%d\n", counter);
  exit(0);
}

int main(void) {
  signal(SIGINT, Prompter);
  signal(SIGQUIT, Report);

  for(;;);
}
