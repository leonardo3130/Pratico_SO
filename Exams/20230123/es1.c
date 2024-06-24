/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere un programma fifotext che:
* crei una named pipe (FIFO) al pathname indicato come primo (e unico) argomento.
* apra la named pipe in lettura
* stampi ogni riga di testo ricevuta
* se la named pipe viene chiusa la riapra
* se riceve la riga "FINE" termini cancellando la named pipe.
Esempio:
fifotext /tmp/ff
....
se in un altra shell si fornisce il comando: "echo ciao > /tmp/ff", fifotext stampa ciao e rimane in attesa
(questo esperimento si può provare più volte). Con il comando "echo FINE > /tmp/ff" fifotext termina.
*/


#include <stdio.h>
#include <stdlib.h>
// #include <math.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
// #include <dirent.h>
#include <sys/stat.h>
// #include <sys/timerfd.h>
// #include <sys/epoll.h>
// #include <time.h>

int main(int argc, char *argv[])
{
  //apertura fifo al path specificato
  mkfifo(argv[1], 0777);

  //open in lettura
  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("openat");
    exit(EXIT_FAILURE);
  }

  char line[512];
  int num_read;

  do {
    num_read = read(fd, line, sizeof(line) - 1);
    if (num_read == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    else if (num_read > 0) {
      line[num_read] = '\0';
      printf("%s\n", line);
    }
    else {
      //other side is closed --> 0 means EOF
      close(fd);
      fd = open(argv[1], O_RDONLY);
      if (fd == -1) {
        perror("openat");
        exit(EXIT_FAILURE);
      }
      line[0] = '\0';
    }
  } while(strcmp(line, "FINE\n") != 0);

  // Close the FIFO
  if (close(fd) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

