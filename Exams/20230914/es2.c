/*
Rielaborare l'esercizio precedente per fare in modo che il vettore e le stringhe dei nomi siano allocati
con una unica operazione malloc (in modo da poter liberare lo spazio usato da vreaddir con una unica
operazione free.)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX 100

char **vreaddir(const char *path) {
  DIR *dir;
  struct dirent *directory_entry;

  dir = opendir(path);

  if(!dir) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }
  
  int dir_len = 0;


  while ((directory_entry = readdir(dir)) != NULL) {
    if(strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0 || directory_entry->d_type == DT_DIR)
      continue;
    dir_len++;
  }

  //allocazione memoria per l'array di stringhe
  int str_len = 50;
  int total_len = sizeof(char *) * (dir_len + 1) + sizeof(char) * (dir_len) * (str_len + 1);
  char **names = (char **)malloc(total_len); //+1 = NULL

  closedir(dir);

  dir = opendir(path);
  int i = 0;
  int len = 0;

  while ((directory_entry = readdir(dir)) != NULL) {
    if(strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0 || directory_entry->d_type == DT_DIR)
      continue;
    len = strlen(directory_entry->d_name);

    //allocazione di memoria per la stringa
    names[i] = (char *)((names + dir_len + 1) + i * (str_len + 1)); 
    //la riga sopra inizializza il puntatore:
    //(names + dir_len + 1) "salta" i puntatori a puntatori di char
    //i * (str_len + 1) serve per "andare" alla stringa successiva
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
  free(strings);

  return EXIT_SUCCESS;
}
