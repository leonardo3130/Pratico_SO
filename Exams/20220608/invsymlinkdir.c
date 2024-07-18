/*
Estendere il programma invsymlink: se il parametro è una directory e non un file
allora tutti i link simbolici presenti nella directory devono venir
"invertititi".
*/
#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 4096

void copyFile(char *dest, char *src) {
  int fd1 = open(src, O_RDONLY);
  int fd2 = open(dest, O_CREAT | O_APPEND | O_WRONLY, 0666);
  char buf[MAX];
  int n = 0;

  while ((n = read(fd1, buf, MAX)) > 0) {
    write(fd2, buf, n);
  }
  close(fd1);
  close(fd2);
}

int invertLink(char *path) {
  struct stat buf;
  int x = stat(path, &buf);
  if (!S_ISLNK(buf.st_mode)) {
    // ottengo il path del file puntato relativo alla locazione del linl
    char buf[MAX];
    ssize_t size = readlink(path, buf, MAX);
    if (size == -1) {
      perror("readlink");
      exit(EXIT_FAILURE);
    }

    // ottengo il path assoluto del file puntato
    char abspath[MAX];
    realpath(buf, abspath);
    printf("abspath: %s\n", abspath);

    // path assoluto del link
    char link_abspath[MAX];
    if (path[0] != '/') {
      char cwd[MAX / 2];
      getcwd(cwd, MAX);
      sprintf(link_abspath, "%s/%s", cwd, path);
      printf("link abspath: %s\n", link_abspath);
    } else {
      strcpy(link_abspath, path);
    }

    // ora rimane solo lo scmabio
    //  copia del file fisico
    unlink(link_abspath);            // cancello il link
    copyFile(link_abspath, abspath); // copio il file, dove prima c'era il link
    unlink(abspath);                 // cancello il file vecchio
    symlink(link_abspath, abspath);  // creo il link dove stava che punta al
    // file che sta dove stava il link
    return 0;
  } else {
    printf("Not a link");
    return -1;
  }
}

int main(int argc, char *argv[]) {
  struct dirent *entry;
  struct stat buf;
  int x = lstat(argv[1], &buf);
  if (S_ISDIR(buf.st_mode)) {
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
      perror("fdopendir");
      exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        char entrypath[MAX];
        printf("\n%s", entry->d_name);
        snprintf(entrypath, MAX, "%s/%s", argv[1], entry->d_name);
        invertLink(entrypath);
      }
    }
  } else {
    printf("qaa");
    invertLink(argv[1]);
  }

  return EXIT_SUCCESS;
}
