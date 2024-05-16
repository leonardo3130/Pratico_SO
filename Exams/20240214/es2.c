/*
Il programma run_name deve cercare nel sottoalbero della directory corrente tutti i file eseguibili con
un nome file specifico (primo parametro di run_name) e li deve mettere in esecuzione uno dopo l'altro
passando i successivi parametri.
Ad esempio il comando:
./run_name testprog a b c
deve cercare i file eseguibili chiamati testprog nell'albero della directory corrente. Poniamo
siano ./testprog, ./dir1/testprog, ./dir/dir3/testprog, run_name deve eseguire
testprog a b c
per 3 volte. Nella prima esecuzione la working directory deve essere la dir corrente '.', la seconda
deve avere come working directory './dir1' e la terza './dir2/dir3'.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_BUF_SIZE 1024

void run_name(char *file_name, char *path, char *arguments[]) {
  struct dirent *directory_entry;
  char new_path[1000];

  DIR *dir;

  dir = opendir(path);

  if(!dir) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  while ((directory_entry = readdir(dir)) != NULL) {
    if(strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
      continue;
    
    snprintf(new_path, sizeof(new_path), "%s/%s",path, directory_entry->d_name);
    if(directory_entry->d_type == DT_DIR) {
      run_name(file_name, new_path, arguments);
    } else {
      if(strcmp(file_name, directory_entry->d_name) == 0 && access(new_path, X_OK) == 0) { //controlla che new_path abbia permesso da eseguibile
        //esegui l'eseguibile
        //cwd = path !!
        pid_t fork_retvalue;
        switch (fork_retvalue = fork()) {
          case 0:
            execvp(new_path, arguments);
            break;
          default:
            int status;
			      waitpid(fork_retvalue, &status, 0); //aspetta la fine
            break;
          case -1:
            perror("fork");
            break;
        }
      }
    }
  }
  closedir(dir);
}

int main(int argc, char *argv[])
{
  run_name(argv[1], ".", argv + 1);
  return EXIT_SUCCESS;
}
