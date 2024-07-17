#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/signalfd.h>

/*
Se guardi s1 vedi che handler di SIGUSR2 viene eseguito prima della write nell'handler di SIGUSR1. 
Mentre in s2, anche se arriva SIGUSR2 mentre si sta gestendo SIGUSR1, si finisce l'esecuzione e 
poi si passa al segnale successivo. Diciamo che è un buon modo per gestire un segnale alla volta.
*/

int main(void) {
  int sfd;
  ssize_t s;
  sigset_t mask;
  struct signalfd_siginfo fdsi;

  /**
   * Questo frammento di codice inizializza una maschera dei segnali e aggiunge due segnali, SIGUSR1 e SIGUSR2, alla maschera.
   * La funzione sigemptyset() inizializza la maschera dei segnali come vuota, mentre la funzione sigaddset() aggiunge i segnali specificati alla maschera.
   */
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);
  sigaddset(&mask, SIGUSR2);

  /* Block signals so that they aren't handled
     according to their default dispositions. */

  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
    err(EXIT_FAILURE, "sigprocmask");

  sfd = signalfd(-1, &mask, 0);
  if (sfd == -1)
    err(EXIT_FAILURE, "signalfd");

  raise(SIGUSR1);   // serve a generare il segnale SIGUSR1
  for (;;) {
    s = read(sfd, &fdsi, sizeof(fdsi));
    if (s != sizeof(fdsi))
      err(EXIT_FAILURE, "read");

    // gestione dei segnali lanciati nel file descriptor sfd
    if (fdsi.ssi_signo == SIGUSR1) {
      raise(SIGUSR2);
      printf("Got SIGUSR1\n");
    } else if (fdsi.ssi_signo == SIGUSR2) {
      printf("Got SIGUSR2\n");
      exit(EXIT_SUCCESS);
    } else {
      printf("Read unexpected signal\n");
    }
  }
}
