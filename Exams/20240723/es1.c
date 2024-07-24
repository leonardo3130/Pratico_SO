#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
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

int main(int argc, char *argv[]) {

  DIR *dir = opendir("./...");
  if (!dir && ENOENT == errno) {
    /* Directory does not exist. */
    if (mkdir("./...", 0777) == -1) {
      perror("mkdir");
      exit(EXIT_FAILURE);
    }
  }
  closedir(dir);
  dir = opendir(".");
  if (!dir) {
    perror("mkdir");
    exit(EXIT_FAILURE);
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
        entry->d_type == DT_REG) {

      char filepath[MAX];
      char filepath_sub[MAX];
      snprintf(filepath, MAX, "%s/%s", ".", entry->d_name);
      snprintf(filepath_sub, MAX, "%s/.../%s", ".", entry->d_name);
      // rename per la sostituzione
      rename(filepath, filepath_sub);
      // symlink per link simbolico
      symlink(filepath_sub, filepath);
    }
  }

  return EXIT_SUCCESS;
}
