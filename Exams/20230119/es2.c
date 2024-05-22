/*
Estendere il programma stdin2pipe in modo da consentire più di due righe di input.
Per esempio, dato il file cmds che contiene 4 righe
ls -l
awk '{print $1}'
sort
uniq
l'esecuzione di:
stdin2pipe < cmds
sia equivalente al comando:
ls -l | awk '{print $1}' | sort | uniq'
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
#define MAX 100

int main(int argc, char *argv[])
{
  
  printf("hello");
  //programma deve funzionare con redirezione --> lettura standard input  
  char buffer[MAX];
  char **list;
  int i = 0;
  do {
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if(buffer) 
      strcpy(list[i], buffer);
    else 
      strcpy(list[i], "");
    i++;
  } while(buffer);

  //command: "comando1 parametri comando 1 | ...... | comando n parametri comando n"
  int len = i;
  i = 0;
  char command[MAX * MAX];
  strcpy(command, list[i]);
  while (strcmp("", list[i + 1]) != 0) {
    strcat(command, " | ");
    strcat(command, list[i + 1]);
    i++;
  }
  //snprintf(command, sizeof(command), "%s | %s", buffer1, buffer2);

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
