#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 1024

void execute_file(char *filename) {
  char line[MAX];

  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  char buf[MAX];
  int r = read(fd, buf, MAX);
  if (r == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  int k = 1;
  int c = 0;
  int s;
  char *arg0 = NULL;
  for (int j = 0; j < r; j++) {
    if (buf[j] == '\n') {
      buf[j] = '\0';
      if (c == 0) {
        s = j + 1;
        arg0 = malloc(j + 1); // j + 1 --> da 0 a j
        strncpy(arg0, buf, j);
        arg0[j] = '\0';
      }
      c++;
    }
  }

  // non c + 1 poiché non considero la prima riga come argv
  char **args = malloc(sizeof(char *) * c);
  args[0] = &buf[s];
  for (int j = s; j < r; j++) {
    if (buf[j] == '\0') {
      args[k++] = &buf[j + 1];
    }
  }
  args[c - 1] = NULL;

  if (fork() == 0) {
    execv(arg0, args);
  }
  free(args);
}

void handle_events(int fd, int wd, char *dir) {
  char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
  const struct inotify_event *event;
  ssize_t len;

  /* Loop while events can be read from inotify file descriptor. */
  for (;;) {
    /* Read some events. */
    len = read(fd, buf, sizeof(buf));
    if (len == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    if (len <= 0)
      break;
    /* Loop over all events in the buffer. */
    for (char *ptr = buf; ptr < buf + len;
         ptr += sizeof(struct inotify_event) + event->len) {
      // calcolo corretto degli offset dei vari eventi
      // The len field counts all of the bytes in name, including the null
      // bytes; the length of each inotify_event structure is thus
      // sizeof(struct inotify_event)+len.

      // creazione puntatore con casting
      event = (const struct inotify_event *)ptr;

      if (wd == event->wd) {
        // aggiunta file --> l'evento ci interessa
        char *path = malloc(strlen(event->name) + 2 + strlen(dir));
        strcat(path, dir);
        strcat(path, "/");
        strcat(path, event->name);
        execute_file(path);
        unlink(path); // rimuove file
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int inotify_fd = inotify_init1(IN_CLOEXEC);
  if (inotify_fd == -1) {
    perror("inotify_init");
  }

  int wd = inotify_add_watch(inotify_fd, argv[1], IN_CREATE | IN_MOVED_TO);
  if (wd == -1) {
    perror("inotify_add_watch");
  }
  handle_events(inotify_fd, wd, argv[1]);
  return EXIT_SUCCESS;
}
