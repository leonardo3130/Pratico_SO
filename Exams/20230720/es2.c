/*
Esercizio 2: Linguaggio C: 10 punti
Il programma cprlt ha 3 paramentri: un tempo in secondi da epoch, una directory sorgente e una di
destinazione. es:
cprl 1689577839 a b
Il programma si deve comportare come cprl dell'esercizio 1 con la differenza che i file con tempo di
ultima modifica precedente al tempo indicato nel parametro vengono collegati con link fisici gli altri
devono essere copiati
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

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

void copyFileSystem(char *s1, char *s2, time_t time) {
  DIR *dir1, *dir2;
  struct dirent *dir_entry; //directory entry --> lettura contenuto cartella 
  
  printf("%s ", s1);
  printf("%s ", s2);
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
    if(strcmp(".", dir_entry->d_name) == 0 || strcmp("..", dir_entry->d_name) == 0)
      continue;
    switch (dir_entry->d_type) {
      case DT_REG:
        //file
        old_path = concatenateDirectoryToFile(s1, dir_entry->d_name);
        new_path = concatenateDirectoryToFile(s2, dir_entry->d_name);
        struct stat file_stat; 

        if(stat(old_path, &file_stat) == -1) {
          perror("stat");
          exit(EXIT_FAILURE);
        }

        time_t last_mod = file_stat.st_mtime;
        if(last_mod < time) {
          //link fisico
          if(link(old_path, new_path) == -1) {
            perror("link");
            exit(EXIT_FAILURE);
          }
          free(old_path);
          free(new_path);
        } else {
          //copia file
          FILE* old;
          FILE* new;
          char buf[2048];

          old = fopen(old_path, "rb"); //read binary
          if(old == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
          }

          new = fopen(new_path, "wb"); //read binary
          if(new == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
          }
          
          size_t read, written;
          while ((read = fread(buf, 1, 2048, old)) > 0) {
            written = fwrite(buf, 1, read, new);
            if(written != read) {
              perror("fwrite");
              exit(EXIT_FAILURE);
            }
          }
          free(old_path);
          free(new_path);

          fclose(old);
          fclose(new);
        }
        break;
      case DT_DIR:
        //directory
        old_path = concatenateDirectoryToFile(s1, dir_entry->d_name);
        new_path = concatenateDirectoryToFile(s2, dir_entry->d_name);
        if(mkdir(new_path, 0777) == -1) {
          perror("mkdir");
          exit(EXIT_FAILURE);
        }
        copyFileSystem(old_path, new_path, time); //directory --> chiamata ricorsiva
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
  copyFileSystem(argv[2], argv[3], atoi(argv[1]));
  return EXIT_SUCCESS;
}
