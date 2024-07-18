#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 4096

void run_file(char *file_path) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *args[16] = {NULL};
  int arg_index = 0;
  pid_t pid;
  while ((read = getline(&line, &len, file)) != -1) {
    if (strcmp(line, "\n") == 0) {
      pid = fork();
      switch (pid) {
      case -1:
        perror("fork");
        exit(EXIT_FAILURE);
      case 0:
        char **ptr = args;
        ptr++;
        execv(args[0], ptr);
        perror("execvp");
        exit(EXIT_FAILURE);
        break;
      default:
        waitpid(pid, NULL, 0);
        arg_index = 0;
        break;
      }
    } else {
      // preparazione array di args
      line[strcspn(line, "\n")] = '\0';
      args[arg_index] = malloc(strlen(line) + 1);
      strncpy(args[arg_index], line, strlen(line) + 1);
      arg_index++;
    }
  }
  fclose(file);
}

void checkFileAdd(char *dir_path) {
  int inotify_fd = inotify_init1(IN_CLOEXEC);
  if (inotify_fd == -1) {
    perror("inotify_ini1");
    exit(EXIT_FAILURE);
  }

  int wd = inotify_add_watch(inotify_fd, dir_path, IN_CREATE | IN_MOVED_TO);
  if (wd == -1) {
    perror("inotify_add_watch");
    exit(EXIT_FAILURE);
  }

  char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
  const struct inotify_event *event;
  ssize_t len;
  for (;;) {

    len = read(inotify_fd, buf, sizeof(buf));
    if (len == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    /* Loop over all events in the buffer. */

    for (char *ptr = buf; ptr < buf + len;
         ptr += sizeof(struct inotify_event) + event->len) {

      event = (const struct inotify_event *)ptr;

      if (wd == event->wd) {
        // esegui i comandi nel file
        char *file_path = malloc(strlen(dir_path) + strlen(event->name) + 2);
        sprintf(file_path, "%s/%s", dir_path, event->name);
        run_file(file_path);
        unlink(file_path);
        free(file_path);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  checkFileAdd(argv[1]);
  return EXIT_SUCCESS;
}
