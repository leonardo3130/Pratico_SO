/*
Il programma fifosig è una estensione di fifotext. Le righe che riceve
attraverso la named pipe sono composte da due numeri, il pid di un processo e il
numero di un segnale. Per ogni riga correttamente formata il segnale indicato
viene mandato al processo indicato dal pid. In un esempio simile la precedente
il comando "echo 12345 15 > /tmp/ff" deve causare l'invio del segnale 15 al
processo 12345. Scrivere il programma fifosig e un programma di prova che scriva
nella pipe il proprio pid e il numero di SIGUSR1 e controlli di ricevere
SIGUSR1.
*/

#include <dirent.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // apertura fifo al path specificato
  mkfifo(argv[1], 0777);

  // open in lettura
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  char line[512];
  int num_read;

  do {
    num_read = read(fd, line, sizeof(line) - 1);
    if (num_read == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    } else if (num_read > 0) {
      line[num_read] = '\0';

      int pid;
      int signal;
      sscanf(line, "%d %d", &pid, &signal);
      printf("%d %d\n", pid, signal);
      if (kill(pid, signal) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
      }
    } else {
      // other side is closed --> 0 means EOF
      close(fd);
      fd = open(argv[1], O_RDONLY);
      if (fd == -1) {
        perror("openat");
        exit(EXIT_FAILURE);
      }
      line[0] = '\0';
    }
  } while (strcmp(line, "FINE\n") != 0);

  // Close the FIFO
  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

// lettura da fifo:
// https://docs.oracle.com/cd/E19455-01/805-7478/pipe6-4/index.html
