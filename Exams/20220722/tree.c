#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 4096

// va realizzato com fdopendir e openat, senza concatenare stringhe

void tree(int dfd, int tabs) {
  DIR *dir = fdopendir(dfd);
  if (dir == NULL) {
    perror("fdopendir");
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      for (int i = 0; i < tabs; i++) {
        printf("  ");
      }
      printf("%s\n", entry->d_name);
      int fd = openat(dfd, entry->d_name, O_RDONLY);
      if (fd == -1) {
        perror("openat");
        return;
      }
      if (entry->d_type == DT_DIR)
        tree(fd, tabs + 1);
      close(fd);
    }
  }
}

int main(int argc, char *argv[]) {
  int dfd = open(argv[1], O_RDONLY);
  if (dfd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  tree(dfd, 0);
  close(dfd);
  return EXIT_SUCCESS;
}
