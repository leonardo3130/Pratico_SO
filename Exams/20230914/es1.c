/*
Scrivere la funzione:
char **vreaddir(const char *path)
che restituisca l'elenco dei nomi dei file in una directory come vettore di
stringhe terminato con un puntatore NULL. (lo stesso formato di argv o envp). Il
vettore e le stringhe dei nomi sono allocate dinamicamente. completare
l'esercizio con un programma principale che testi il corretto funzionamento
della funzione vreaddir.
*/
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 100

char **vreaddir(const char *path) {
  DIR *dir;
  struct dirent *directory_entry;

  dir = opendir(path);

  if (!dir) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  int dir_len = 0;
  int str_len = 0;

  while ((directory_entry = readdir(dir)) != NULL) {
    if (strcmp(directory_entry->d_name, ".") == 0 ||
        strcmp(directory_entry->d_name, "..") == 0 ||
        directory_entry->d_type == DT_DIR)
      continue;
    dir_len++;
  }

  // allocazione memoria per l'array di stringhe --> (per i puntatori char *)
  char **names = (char **)malloc((dir_len + 1) * sizeof(char *)); //+1 = NULL

  closedir(dir);

  dir = opendir(path);
  int i = 0;

  while ((directory_entry = readdir(dir)) != NULL) {
    if (strcmp(directory_entry->d_name, ".") == 0 ||
        strcmp(directory_entry->d_name, "..") == 0 ||
        directory_entry->d_type == DT_DIR)
      continue;
    str_len = strlen(directory_entry->d_name);

    // allocazione di memoria per la stringa
    names[i] =
        (char *)malloc((str_len + 1) * sizeof(char)); //+1 = terminatore stringa
    strncpy(names[i], directory_entry->d_name, str_len + 1);

    i++;
  }
  names[i] = NULL;

  closedir(dir);

  return names;
}

int main(int argc, char *argv[]) {

  char **strings = vreaddir(argv[1]);
  int i = 0;
  while (strings[i] != NULL) {
    printf("%s\n", strings[i]);
    i++;
  }
  i = 0;
  while (strings[i] != NULL) {
    free(strings[i]);
    i++;
  }
  free(strings);

  return EXIT_SUCCESS;
}
