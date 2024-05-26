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

/*#include <stdio.h>
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
  
  //programma deve funzionare con redirezione --> lettura standard input  
  char buffer[MAX];
  char list[MAX][MAX];
  int i = 0;

  while(fgets(buffer, sizeof(buffer), stdin)){
    buffer[strcspn(buffer, "\n")] = '\0';
    if(strlen(buffer) == 0) 
      break;
    strcpy(list[i], buffer);
    i++;
  } 

  //command: "comando1 parametri comando 1 | ...... | comando n parametri comando n"
  char command[MAX * MAX] = "";
  for (int j = 0; j < i; j++){
    if(j > 0)
      strcat(command, " | ");
    strcat(command, list[j]);
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
  
  return EXIT_SUCCESS;
}*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 100

int main(int argc, char *argv[]) {
    char buffer[MAX];
    char *list[MAX];
    int i = 0;

    // Legge i comandi dalla standard input
    while (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Rimuove il newline
        if (strlen(buffer) == 0)
            break;
        list[i] = strdup(buffer); // Copia il comando nella lista
        i++;
    }
    
    int num_commands = i;
    int pipes[MAX][2]; // Array di pipe --> più redirection --> più pipes

    // Creazione delle pipe
    for (i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < num_commands; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Redirezione degli input/output delle pipe
            if (i > 0) {
                // Se non è il primo comando, redirige l'input dalla pipe precedente
                if (dup2(pipes[i-1][0], STDIN_FILENO) == -1) { //chi proverà a leggere STDIN_FILENO, leggerà 
                    // pipes[i-1][0] , 0 = lato lettura della pipe
                    perror("dup2 input");
                    exit(EXIT_FAILURE);
                }
            }
            if (i < num_commands - 1) {
                // Se non è l'ultimo comando, redirige l'output alla pipe successiva
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) { //chi proverà a scrivere STDOUT_FILENO, scriverà
                    // pipes[i][1], 1 = lato scrittura della pipe
                    perror("dup2 output");
                    exit(EXIT_FAILURE);
                }
            }

            // Chiude tutte le pipe non utilizzate
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Divide il comando in argomenti
            char *args[MAX];
            char *token = strtok(list[i], " ");
            int arg_count = 0;
            while (token != NULL) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;
            //char *strtok(char *str, const char *delim) --> tokenizza la stringa, 
            //in questo caso ogni volta che incontra uno spazio

            // Esegue il comando
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            //non serve scrivere su STDOUT e leggere da STDIN --> sono STANDARD
        }
    }

    // Chiude tutte le pipe nel processo padre
    for (i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Aspetta che tutti i figli terminino
    for (i = 0; i < num_commands; i++) {
        wait(NULL);
    }

    // Libera la memoria allocata
    for (i = 0; i < num_commands; i++) {
        free(list[i]);
    }

    return EXIT_SUCCESS;
}
