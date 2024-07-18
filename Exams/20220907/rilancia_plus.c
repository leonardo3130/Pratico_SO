/*
Se si esegue il programa dell'esercizio 1 in questo modo:
rilancia cat /etc/hostname
il comando (cat) viene eseguito ripetutamente all'infinito.
Modificare il programma rilancia per fare in modo che se l'esecuzione del
programma (in questo caso cat /etc/hostname) dura meno di un secondo non si
proceda alla riattivazione.
*/
#include <sys/wait.h>
#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define MAX 4096

int main(int argc, char *argv[]) {
  int pid;
  int flag = 1;
  int status;
  struct timeval t1, t2;

  while (flag) {
    gettimeofday(&t1, NULL);
    switch (pid = fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      char **ptr = argv;
      ptr++;
      // gettimeofday(&t1, NULL); // parte il cronometro --> non va bene qua
      // --> settata solo per il figlio, ma non per il padre che deve misurare
      execvp(argv[1], ptr);
      perror("execlp");
      exit(EXIT_FAILURE);
    default:
      waitpid(pid, &status, 0);
      gettimeofday(&t2, NULL); // fine cronometro
      if (WIFEXITED(status) && WEXITSTATUS(status) == 0 &&
          WIFSIGNALED(status) == 0 && t2.tv_sec - t1.tv_sec >= 1) {
        flag = 1;
      } else {
        flag = 0;
      }
    }
  }
  return EXIT_SUCCESS;
}
