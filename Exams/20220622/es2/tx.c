/*
Estendere i programmi dell'es.1 per consentire il trasferimento di stringhe di
lunghezza arbitraria (iterando il procedimento 8 caratteri alla volta).
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

  int l = strlen(argv[2]);
  int i = 0;
  int ret;
  int64_t msg;
  while (l > 0) {
    msg = *((int64_t *)&argv[2][i]);
    val.sival_ptr = 0;
    val.sival_ptr = (void *)msg;

    ret = sigqueue(rx_pid, SIGRTMIN + 1, val);
    // servono segnali realtime, altrimenti non vengono messi in coda
    //--> vanno da SIGRTMIN a SIGRTMAX
    if (ret == -1) {
      perror("sigqueue");
      exit(EXIT_FAILURE);
    }
    l -= MAX_MSG;
    i += MAX_MSG;
  }

  return EXIT_SUCCESS;
}
