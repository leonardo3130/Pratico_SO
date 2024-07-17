/*
Scrivere due programmi C, tx e rx: tx deve trasferire a rx stringhe di max 8
caratteri usando i valori assegnati ai segnali (il parametro value della
funzione sigqueue). Il programma rx deve per prima cosa stampare il proprio pid
e attendere segnali. ill programma tx ha due parametri, il pid did rx e il
messaggio. es: tx 22255 sigmsg (posto che rx sia in esecuzione con pid 22255, rx
deve stampare sigmsg).
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

  write(STDOUT_FILENO, msg, MAX_MSG);
  write(STDOUT_FILENO, "\n", 1);
}

int main(int argc, char *argv[]) {
  struct sigaction action;

  action.sa_sigaction = rx_handler;
  action.sa_flags = SA_SIGINFO;
  int r = sigaction(SIGUSR1, &action, NULL);
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
