/*
Scrivere il programma stdin2pipe che prenda in input due righe di testo, ogni riga contiene un
comando coi rispettivi parametri. Il programma deve lanciare entrambi i comandi in modo tale
l'output del primo diventi input del secondo.
Per esempio, dato il file cmds:
ls -l
awk '{print $1}'
l'esecuzione di:
stdin2pipe < cmds
sia equivalente al comando:
ls -l | awk '{print $1}'
*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX 1024

int main(int argc, char *argv[])
{
  
  //programma deve funzionare con redirezione --> lettura standard input  
  char buffer1[MAX];
  char buffer2[MAX];
  if(fgets(buffer1, sizeof(buffer1), stdin) == NULL) {
    perror("fgets");
    exit(EXIT_FAILURE);
  }

  buffer1[strcspn(buffer1, "\n")] = '\0';
  
  if(fgets(buffer2, sizeof(buffer2), stdin) == NULL) {
    perror("fgets");
    exit(EXIT_FAILURE);
  }

  buffer2[strcspn(buffer2, "\n")] = '\0';  
  //strcspn ritorna la prima pos in cui trova uno dei caratteri presenti nell'altro parametro

  //command: "comando1 parametri comando | comando2 parametri comando"
  char command[MAX * 2 + 4]; 
  snprintf(command, sizeof(command), "%s | %s", buffer1, buffer2);

  //esecuzione del comando
  char *args[] = {"/bin/bash", "-c", command, NULL};
  //invece di stare a separare gli elementi del comando per passarli a execvp
  //li passo in esecuzione a bash specificando con -c che il parametro dopo è il comando
  int pid, status;
  switch (pid = fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      execvp(args[0], args);
      break;
    default:
      wait(&status);
      break;
  }
  execvp(args[0], argv);

  printf("still here");
  
  return EXIT_SUCCESS;
}
