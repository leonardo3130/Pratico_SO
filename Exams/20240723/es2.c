#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 4096

// controllo che il link sia riferito a ...
int isValid(char *str) {
  int cmp = strncmp(str, "./...", 5);
  if (cmp == 0)
    return 1;
  return 0;
}

int main(int argc, char *argv[]) {

  DIR *dir = opendir(".");
  if (!dir) {
    perror("mkdir");
    exit(EXIT_FAILURE);
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
        entry->d_type == DT_LNK) {
      char link_target[MAX];

      char linkpath[MAX];
      snprintf(linkpath, MAX, "%s/%s", ".", entry->d_name);

      // lettura link nella cwd
      if (readlink(linkpath, link_target, MAX) == -1) {
        perror("readlink");
        exit(EXIT_FAILURE);
      }
      if (isValid(link_target) == 0)
        continue;

      unlink(linkpath);
      // rename per la sostituzione
      rename(link_target, linkpath);
    }
  }

  return EXIT_SUCCESS;
}
