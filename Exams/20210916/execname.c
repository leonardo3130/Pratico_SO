/*
Creare una directory chiamata exec. Scrivere un programma execname che se viene
aggiunto un file nela directory exec interpreti il nome del file come comando
con parametri, lo esegua e cancelli il file. es: sopo aver lanciato execname:
execname exec
a seguito di questo comando:
touch 'exec/echo ciao mare'
il programma stampa:
ciao mare
(consiglio, usare inotify)
*/
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

void run_file(char *path, const char *name) {
  char var_name[MAX];
  strncpy(var_name, name, strlen(name));
  int c = 1;
  int l = strlen(var_name); // MAI LASCIARE strlen(name) nel for !!!
  for (int i = 0; i < l; i++) {
    // printf("\n%d %c", i, var_name[i]);
    if (name[i] == ' ') {
      var_name[i] = '\0'; // per realizzare array di argomenti
      c++;
    }
  }

  char **args = malloc(sizeof(char *) * (c + 1)); // +1 per NULL
  args[0] = var_name;
  int k = 1;
  for (int i = 0; i < l; i++) {
    if (var_name[i] == '\0') {
      args[k++] = &var_name[i + 1];
    }
  }
  args[c] = NULL;
  // for (int i = 0; i < argc; i++) {
  //   printf("%s\n", args[i]);
  // }
  if (fork() == 0) {
    execvp(args[0], args);
  }
  free(args);
}

void execFileNames(char *path) {

  int inotify_fd = inotify_init1(IN_CLOEXEC);
  if (inotify_fd == -1) {
    perror("inotify_ini1");
    exit(EXIT_FAILURE);
  }

  int wd = inotify_add_watch(inotify_fd, path, IN_CREATE | IN_MOVED_TO);
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
        char *file_path = malloc(strlen(path) + strlen(event->name) + 2);
        sprintf(file_path, "%s/%s", path, event->name);
        run_file(file_path, event->name);
        unlink(file_path);
        free(file_path);
      }
    }
  }

  inotify_rm_watch(inotify_fd, wd);
  close(inotify_fd);
}

int main(int argc, char *argv[]) {
  char path[MAX] =
      "/home/leonardo/Desktop/Uni/2/Sistemi/Pratico_SO/Exams/20210916/exec";
  execFileNames(path);
  return EXIT_SUCCESS;
}
