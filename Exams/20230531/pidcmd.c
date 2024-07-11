/*
Scriveere un programma pidcmd che stampi i pid dei processi attivi lanciati con
una specifica riga di comando. (Devono coincidere tutti gli argomenti) es: ll
comando "pidcmd less /etc/hostname" deve stampare il numero di processo dei
processi attivi che sono stati lanciati con "less /etc/hostname" (hint: cercare
nelle directory dei processi in /proc i "file" chiamati cmdline)
*/

#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 4096

int notPID(char *filename) {
  for (int i = 0; i < strlen(filename); i++) {
    if (filename[i] < '0' || filename[i] > '9')
      return 1;
  }
  return 0;
}

char *strncat_with_EOS(char *dest, char *src, size_t n1, ssize_t n2) {
  for (int i = 0; i < n2; i++)
    dest[n1 + i] = src[i];

  return dest;
}

int same_cmdline(int fd, char *cmdline, int size) {
  char buffer[MAX];
  int r;
  if ((r = read(fd, buffer, MAX)) == -1) {
    perror("read");
    return 0;
  }
  if (r != size)
    return 0;
  if (memcmp(buffer, cmdline, size) == 0)
    return 1;
  return 0;
}

int check_proc_dir(char *cmdline, int size) {
  int counter = 0;
  DIR *dir;
  dir = opendir("/proc/");
  if (dir == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ||
        notPID(entry->d_name))
      continue;
    char file_path[MAX];
    sprintf(file_path, "/proc/%s/cmdline", entry->d_name);
    int fd = open(file_path, O_RDONLY);
    counter += same_cmdline(fd, cmdline, size);
    if (fd == -1) {
      perror("open");
    }
    close(fd);
  }
  closedir(dir);
  return counter;
}

int main(int argc, char *argv[]) {
  int size = 0;
  // calcolo size per allocare memoria
  for (int i = 1; i < argc; i++) {
    size += strlen(argv[i]) + 1;
  }
  // allocazione
  char *str = malloc(sizeof(char) * size);
  size = 0;
  for (int i = 1; i < argc; i++) {
    strncat_with_EOS(str, argv[i], size, strlen(argv[i]) + 1);
    size += strlen(argv[i]) + 1;
  }

  printf("%d\n", check_proc_dir(str, size));
  free(str);

  // str contiene la cmdline
}
