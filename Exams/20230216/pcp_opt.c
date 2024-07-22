
/*
usando “getopt” consentire di scegliere il grado di parallelismo voluto:
pcp -j 8 file1 file2
deve creare 8 processi, ogni processo copia 1/8 del file.
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

void pcp(int rfd, int wfd, int file_len, int par) {
  char buf[file_len];
  int r, total = 0;

  int unit = file_len / par;
  for (int i = 0; i < par; i++) {
    if (fork() == 0) {
      lseek(rfd, total, SEEK_SET);
      lseek(wfd, total, SEEK_SET);
      if (i == par - 1) {
        r = read(rfd, buf, unit + par);
        write(wfd, buf, r);
      } else {
        r = read(rfd, buf, unit);
        write(wfd, buf, r);
      }
      exit(EXIT_SUCCESS);
    } else {
      total += unit; // importante che solo padre aggiorni total
    }
  }

  // padre deve aspettare i figli
  // se il padre dovesse terminare
  // prima, morirebbero tutti i figli
  // --> rischio di non scrittura/lettura file
  for (int i = 0; i < par; i++) {
    wait(NULL);
  }
}

int main(int argc, char *argv[]) {
  int opt;
  int j_value = -1;

  // Parse command line options
  while ((opt = getopt(argc, argv, "j:")) != -1) {
    switch (opt) {
    case 'j':
      j_value = atoi(optarg);
      break;
    default: /* '?' */
      fprintf(stderr, "Usage: %s -j X param1 param2\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  // Ensure -j was provided and exactly two additional parameters are present
  if (j_value == -1 || optind + 2 != argc) {
    fprintf(stderr, "Usage: %s -j X param1 param2\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Retrieve param1 and param2
  char *param1 = argv[optind];
  char *param2 = argv[optind + 1];

  int rfd, wfd;
  int size, n;
  char buf[MAX];
  rfd = open(param1, O_RDONLY);
  if (rfd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }
  while ((n = read(rfd, buf, MAX)) > 0) {
    size += n;
  }
  close(rfd);
  rfd = open(param1, O_RDONLY);
  if (rfd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }
  wfd = open(param2, O_WRONLY | O_CREAT, 0777);
  if (wfd == -1) {
    perror("open");
    return EXIT_FAILURE;
  }
  pcp(rfd, wfd, size, j_value);
  close(rfd);
  close(wfd);
  return EXIT_SUCCESS;
  return 0;
}
