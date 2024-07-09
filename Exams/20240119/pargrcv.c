#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 1024

int main(int argc, char *argv[]) {
  char str[MAX];
  mkfifo(argv[1], 0777);

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  int l = read(fd, str, MAX - 1);
  // bloccante se non c'è nulla da leggere

  if (l == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  } else if (l > 0) {
    // str[l] = '\0';
    printf("%d\n", l);
    printf("%s\n", str);

    // calcolo numero di argomenti
    int c = 0;
    for (int i = 0; i < l; i++) {
      if (str[i] == '\0') {
        c++;
      }
    }
    // allocazione memoria array di stringhe (argomenti comando)
    char **args = malloc(sizeof(char *) * (c + 1));
    args[0] = str; // str da intender come &str[0]
    int k = 1;
    for (int i = 0; i < l; i++) {
      if (str[i] == '\0') {
        args[k++] = &str[i + 1];
      }
    }
    args[c] = NULL; // la lista di args deve terminare con NULL
    execvp(args[0], args);
  }
  close(fd);

  return EXIT_SUCCESS;
}
