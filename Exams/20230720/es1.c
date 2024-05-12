/*
 Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere un programma cprl che si comporti come il comando "cp -rl". cprl ha due parametri:
cprl a b
deve copiare l'intera struttura delle directory dell'albero che ha come radice a in un secondo albero
con radice b. I file non devono essere copiati ma collegati con link fisici.
(l'operazione deve essere fatta dal codice C, senza lanciare altri programmi/comandi)
Esercizio 2: Linguaggio C: 10 punti
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

char* concatenateDirectoryToFile(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2;

    char* result = (char*)malloc(total_len + 1 + 1); // +1 for null terminator, +1 for /
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(result, str1);
    strcat(result, "/");
    strcat(result, str2);

    return result;
}

void copyFileSystem(char *s1, char *s2) {
  DIR *dir1, *dir2;
  struct dirent *dir_entry; //directory entry --> lettura contenuto cartella 
  
  dir1 = opendir(s1);
  if (dir1 == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }
  
  dir2 = opendir(s2);
  if (dir2 == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  char *old_path;
  char *new_path;
  while ((dir_entry = readdir(dir1)) != NULL) {
    printf("%s ", dir_entry->d_name);
    if(strcmp(".", dir_entry->d_name) == 0 || strcmp("..", dir_entry->d_name) == 0)
      continue;
    switch (dir_entry->d_type) {
      case DT_REG:
        //file
        old_path = concatenateDirectoryToFile(s1, dir_entry->d_name);
        new_path = concatenateDirectoryToFile(s2, dir_entry->d_name);
        if(link(old_path, new_path) == -1) {
          perror("link");
          exit(EXIT_FAILURE);
        }
        free(old_path);
        free(new_path);
        break;
      case DT_DIR:
        //directory
        old_path = concatenateDirectoryToFile(s1, dir_entry->d_name);
        new_path = concatenateDirectoryToFile(s2, dir_entry->d_name);
        if(mkdir(new_path, 0777) == -1) {
          perror("mkdir");
          exit(EXIT_FAILURE);
        }
        copyFileSystem(old_path, new_path); //directory --> chiamata ricorsiva
        free(old_path);
        free(new_path);
      default:
        break;
    }
  }

  closedir(dir1);
  closedir(dir2);
}

int main(int argc, char *argv[]) {
  copyFileSystem(argv[1], argv[2]);
  return EXIT_SUCCESS;
}
