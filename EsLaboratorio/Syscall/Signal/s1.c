#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handler1() {
  raise(SIGUSR2);
  char *s = "Handler1\n";
  write(1, s, strlen(s));
}

void handler2() {
  char *s = "Handler2\n";
  write(1, s, strlen(s));
}

int main() {
  struct sigaction act = {0};

  act.sa_sigaction = &handler1;
  sigaction(SIGUSR1, &act, NULL);

  act.sa_sigaction = &handler2;
  sigaction(SIGUSR2, &act, NULL);

  raise(SIGUSR1);
  char *s = "After singal\n";
  write(1, s, strlen(s));
}
