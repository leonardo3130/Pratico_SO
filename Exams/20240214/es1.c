/*
Scrivere un programma search_name che deve cercare nel sottoalbero della directory corrente tutti i
file eseguibili con un nome file specifico passato come primo e unico parametro indicando per ogni
file il tipo di eseguibile (script o eseguibile binario).
Ad esempio il comando:
./search_name testprog
deve cercare i file eseguibili chiamati testprog nell'albero della directory corrente. Poniamo
siano ./testprog, ./dir1/testprog, ./dir/dir3/testprog, search_name deve stampare:
. /testprog: script
./dir1/testprog: ELF executable
./dir/dir3/testprog: ELF executable
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#define MAX_BUF_SIZE 1024

int isELF(const char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 0;
  }

  char buf[4] = {0};
  if (read(fd, buf, 4) != 4) {
    perror("read");
    close(fd);
    return 0;
  }

  close(fd);

  // Controllo se i primi quattro bit sono quelli degli elf
  if (memcmp(buf, "\x7f\x45\x4c\x46", 4) == 0) //confronta i byte e ritorna 0 se coincidono
    return 1; // ELF
  return 0;
}

int isScript(const char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 0;
  }

  char buf[MAX_BUF_SIZE];
    
  ssize_t n = read(fd, buf, sizeof(buf) - 1);

  if(n < 0) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  buf[n] = '\0';
  close(fd);  
    
  if (strncmp(buf, "#!", 2) == 0) //controlla se il file comincia per #!
    return 1; // Script

  return 0;
}

void search_name(char *file_name, char *path) {
  struct dirent *directory_entry;
  char new_path[1000];

  DIR *dir;

  dir = opendir(path);

  if(!dir) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  while ((directory_entry = readdir(dir)) != NULL) {
    //printf("%s\n", directory_entry->d_name);
    if(strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
      continue;
    
    snprintf(new_path, sizeof(new_path), "%s/%s",path, directory_entry->d_name);
    if(directory_entry->d_type == DT_DIR) {
      search_name(file_name, new_path);
    } else {
      if(strcmp(file_name, directory_entry->d_name) == 0 && access(new_path, X_OK) == 0) { //controlla che new_path abbia permesso da eseguibile
        if(isScript(new_path)) {
          printf("%s: script\n", new_path);

        } else if (isELF(new_path)) {
          printf("%s: ELF executable\n", new_path);

        }
        else {
          printf("%s: executable\n", new_path);
        }
      }
    }
  }
  closedir(dir);
}

int main(int argc, char *argv[])
{
  search_name(argv[1], ".");
  return EXIT_SUCCESS;
}
