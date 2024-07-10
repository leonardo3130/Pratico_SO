#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 1024

char *read_cmdline(char *pid, int *l) {
  char path[MAX];
  printf("%s", pid);
  snprintf(path, MAX, "/proc/%s/cmdline", pid);
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  char *cmdline = malloc(MAX);
  int r = read(fd, cmdline, MAX);
  if (r == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  *l = r;
  close(fd);

  return cmdline;
}

int main(int argc, char *argv[]) {
  int l;
  char *cmdline = read_cmdline(argv[1], &l);
  int c = 0;
  // calcolo numero di argomenti
  for (int i = 0; i < l; i++) {
    if (cmdline[i] == '\0') {
      c++;
    }
  }
  // allocazione memoria array di stringhe (argomenti comando)
  char **args = malloc(sizeof(char *) * (c + 1));
  args[0] = cmdline;
  int k = 1;
  for (int i = 0; i < l; i++) {
    if (cmdline[i] == '\0') {
      args[k++] = &cmdline[i + 1];
    }
  }
  args[c] = NULL;
  execvp(args[0], args);
  return EXIT_SUCCESS;
}
