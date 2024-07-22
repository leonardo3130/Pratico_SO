#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 4096

void stdin2pipe(char *s, int size) {
  int pipefd[2];
  int first_end = 0;
  int argc1, argc2;
  argc1 = argc2 = 0;
  int second_half = 0;
  for (int i = 0; i < size; i++) {
    if (s[i] == '\0') {
      s[i] = '\0';
      if (second_half == 0)
        argc1++;
      else {
        argc2++;
      }
    } else if (s[i] == '\n') {
      s[i] = '\0';
      if (second_half == 0) {
        argc1++;
        first_end = i;
        second_half = 1;
      } else {
        argc2++;
      }
    }
  }

  char **args1 = malloc(sizeof(char *) * argc1 + 1);
  char **args2 = malloc(sizeof(char *) * argc2 + 1);
  int k = 1;
  args1[0] = s;
  args2[0] = &s[first_end + 1];
  for (int i = 0; i < first_end; i++) {
    if (s[i] == '\0')
      args1[k++] = &s[i + 1];
  }
  args1[argc1] = NULL;
  k = 1;
  for (int i = first_end + 1; i < size; i++) {
    if (s[i] == '\0')
      args2[k++] = &s[i + 1];
  }
  args2[argc2] = NULL;

  pipe(pipefd);
  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
    break;
  case 0: // child
    close(pipefd[0]);
    wait(NULL);

    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    execvp(args1[0], args1);

    break;
  default: // father
    close(pipefd[1]);

    dup2(pipefd[0], STDIN_FILENO); // per leggere dal figlio
    close(pipefd[0]);
    execvp(args2[0], args2);

    break;
  }
  free(args1);
  free(args2);
}

int main(int argc, char *argv[]) {
  char buf[MAX];
  int r = read(STDIN_FILENO, buf, MAX);
  if (r == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  stdin2pipe(buf, r);
  return EXIT_SUCCESS;
}
