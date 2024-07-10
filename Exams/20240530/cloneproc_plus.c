#define _GNU_SOURCE
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

char *read_exe(char *pid, int *l) {
  char path[MAX];
  snprintf(path, MAX, "/proc/%s/exe", pid);
  char *exe = malloc(MAX);
  int r = readlink(path, exe, MAX);
  if (r == -1) {
    perror("readlink");
    exit(EXIT_FAILURE);
  }
  *l = r;

  return exe;
}

int main(int argc, char *argv[]) {
  int l1, l2;
  char *cmdline = read_cmdline(argv[1], &l1);
  char *exe = read_exe(argv[1], &l2);
  int c = 0;
  // calcolo numero di argomenti
  for (int i = 0; i < l1; i++) {
    if (cmdline[i] == '\0') {
      c++;
    }
  }
  // allocazione memoria array di stringhe (argomenti comando)
  char **args = malloc(sizeof(char *) * (c + 1));
  args[0] = cmdline;
  int k = 1;
  for (int i = 0; i < l1; i++) {
    if (cmdline[i] == '\0') {
      args[k++] = &cmdline[i + 1];
    }
  }
  args[c] = NULL;

  // calcolo numero elementi del path
  char *env[16];
  char *token = strtok(exe, "/");
  int arg_count = 0;
  while (token != NULL) {
    env[arg_count++] = token;
    token = strtok(NULL, "/");
  }
  env[arg_count] = NULL;
  execvpe(args[0], args, env);
  return EXIT_SUCCESS;
}
