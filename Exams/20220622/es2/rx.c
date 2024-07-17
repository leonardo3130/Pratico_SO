/*
Estendere i programmi dell'es.1 per consentire il trasferimento di stringhe di
lunghezza arbitraria (iterando il procedimento 8 caratteri alla volta).
*/

#include <bits/types/siginfo_t.h>
#include <stdint.h>
#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_MSG 8

void rx_handler(int signum, siginfo_t *info, void *secret) {
  char msg[MAX_MSG];
  // ottengo i 64 bit di intero
  int64_t ptr = (int64_t)info->si_value.sival_ptr;
  for (int i = 0; i < MAX_MSG; i++) {
    msg[i] = ptr % 256; // primi 8 bit
    ptr /= 256;
  }

  // printf("\n%s", msg); --> non è signal safe
  write(STDOUT_FILENO, msg, MAX_MSG);
  write(STDOUT_FILENO, "\n", 1);
}

int main(int argc, char *argv[]) {
  struct sigaction action;

  action.sa_sigaction = rx_handler;
  action.sa_flags = SA_SIGINFO | SA_RESTART;
  int r = sigaction(SIGRTMIN + 1, &action, NULL);
  if (r == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  printf("%d\n", getpid());

  while (1) {
    pause();
  }
  return EXIT_SUCCESS;
}
