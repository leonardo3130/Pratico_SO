/*
Scrivere due programmi C, tx e rx: tx deve trasferire a rx stringhe di max 8
caratteri usando i valori assegnati ai segnali (il parametro value della
funzione sigqueue). Il programma rx deve per prima cosa stampare il proprio pid
e attendere segnali. ill programma tx ha due parametri, il pid did rx e il
messaggio. es: tx 22255 sigmsg (posto che rx sia in esecuzione con pid 22255, rx
deve stampare sigmsg).
*/

#include <stdint.h>
#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_MSG 8

int main(int argc, char *argv[]) {
  union sigval val;
  int rx_pid = atoi(argv[1]);

  int64_t msg = *((int64_t *)argv[2]);
  // il puntatore alla stringa è convertito in un
  // puntatore a un intero di 64 bit (8 caratteri)
  // e in seguito dereferenzio il puntatore per
  // ottenere il valore dell'intero

  val.sival_ptr = (void *)msg;

  int ret = sigqueue(rx_pid, SIGUSR1, val);
  if (ret == -1) {
    perror("sigqueue");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
