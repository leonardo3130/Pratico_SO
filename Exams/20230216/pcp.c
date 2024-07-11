/*
Fare un programma di copia parallelo di file.
pcp file1 file2
pcp deve creare due processi figli; mentre un processo copia la prima meta’ del
file, l'altro deve copiare l’altra meta’.
*/

#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX 4096

void pcp(int rfd, int wfd, int file_len) {
  char buf[file_len];
  int n;
  if (fork() == 0) {
    // figlio scrive la seconda metà
    lseek(rfd, file_len / 2, SEEK_SET);
    lseek(wfd, file_len / 2, SEEK_SET);
    n = read(rfd, buf, file_len / 2);
    write(wfd, buf, n);
  } else {
    // padre scrive la prima metà
    lseek(rfd, 0, SEEK_SET);
    lseek(wfd, 0, SEEK_SET);
    if (file_len % 2)
      n = read(rfd, buf, file_len / 2 + 1);
    else
      n = read(rfd, buf, file_len / 2);
    write(wfd, buf, n);
  }
}

int main(int argc, char *argv[]) {
  int rfd, wfd;
  int size, n;
  char buf[MAX];
  rfd = open(argv[1], O_RDONLY);
  if (rfd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }
  while ((n = read(rfd, buf, MAX)) > 0) {
    size += n;
  }
  close(rfd);
  rfd = open(argv[1], O_RDONLY);
  if (rfd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }
  wfd = open(argv[2], O_WRONLY | O_CREAT, 0777);
  if (wfd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }
  pcp(rfd, wfd, size);
  close(rfd);
  close(wfd);
  return EXIT_SUCCESS;
}
