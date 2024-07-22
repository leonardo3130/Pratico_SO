#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CMD_LEN 1024
#define MAX_CMDS 100

void execute_pipeline(char *cmds[], int n) {
  int i;
  int fd[2];
  pid_t pid;
  int fd_in = STDIN_FILENO; // First command reads from stdin

  pipe(fd); // ok
  for (i = 0; i < n; ++i) {
    // pipe(fd);  //ok

    if ((pid = fork()) == -1) {
      perror("fork");
      exit(1);
    } else if (pid == 0) {
      // legge da fd_in e scrive in fd[1]
      dup2(fd_in, STDIN_FILENO);    // fd_in punta a stdin
      if (i < n - 1) {              // tuti tranne l'ultimo scrivono nella pipe
        dup2(fd[1], STDOUT_FILENO); // fd[1] punta a stdout
      }
      close(fd[0]); // non usata

      // Split the command into arguments
      char *args[MAX_CMD_LEN];
      char *token = strtok(cmds[i], " ");
      int j = 0;
      while (token != NULL) {
        args[j++] = token;
        token = strtok(NULL, " ");
      }
      args[j] = NULL;

      // Execute the command
      execvp(args[0], args); // scrive su stdout che è fd[1]
      perror("execvp");
      exit(1);
    } else {
      wait(NULL);
      close(fd[1]);  // chiudo parte di scrittura (che è stdout)
      fd_in = fd[0]; // fd_in punta all'estremo di fd[0] --> perché fd[0]
      // verrà sovrascritta con pipe(fd)
    }
  }
}

int main() {
  char line[MAX_CMD_LEN];
  char *cmds[MAX_CMDS];
  int n = 0;

  // Read commands from stdin
  while (fgets(line, sizeof(line), stdin) != NULL) {
    // Remove newline character
    line[strcspn(line, "\n")] = '\0';
    cmds[n] = strdup(line); // per non sovrascrivere la memoria di line
    n++;
  }

  if (n == 0) {
    fprintf(stderr, "No commands provided.\n");
    return 1;
  }

  // Execute the commands in a pipeline
  execute_pipeline(cmds, n);

  // Free allocated memory
  for (int i = 0; i < n; ++i) {
    free(cmds[i]);
  }

  return 0;
}
