/*
Scrivere un programma C rilancia: che esegua il programma indicato in argv[1]
con i relativi parametri (in argv[2] e seguenti): es: rilancia tr a-z A-Z esegue
il comando tr coi parametri. Se il programma lanciato termina senza errori (non
per colpa di un segnale e con valore di ritorno 0) rilancia deve rieseguire il
programma (nell'esempio tr) con i medesimi parametri.
*/
#include <sys/wait.h>
#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 4096

int main(int argc, char *argv[]) {
  int pid;
  int flag = 1;
  int status;
  while (flag) {
    switch (pid = fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      char **ptr = argv;
      ptr++;
      execvp(argv[1], ptr);
      perror("execlp");
      exit(EXIT_FAILURE);
    default:
      waitpid(pid, &status, 0);
      if (WIFEXITED(status) && WEXITSTATUS(status) == 0 &&
          WIFSIGNALED(status) == 0) {
        flag = 1;
      } else {
        flag = 0;
      }
    }
  }
  return EXIT_SUCCESS;
}
