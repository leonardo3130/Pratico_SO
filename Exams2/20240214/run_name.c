#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

const char *DIRS_EXECVE[] = {".", "./dir1", "./dir2/dir3"};


// Funzione per verificare se il file è uno script
int is_script(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file)
        return 0;
    
    char buffer[2] = {0};
    fread(buffer, 1, 2, file);
    fclose(file);
    
    return memcmp(buffer, "#!", 2) == 0;
}

// Funzione per determinare se un file è un eseguibile binario ELF
int is_elf(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }
    
    char buffer[5] = {0};
    fread(buffer, 1, 4, file);
    fclose(file);
    
    return memcmp(buffer, "\x7f""ELF", 4) == 0;
}

// Funzione ricorsiva per esplorare le directory
void search_in_directory(const char *dir_name, const char *target_name, const char *args, const char *env) {
    DIR *dir = opendir(dir_name);
    if (!dir) {
        perror("opendir");
        return;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // se fa uno di sti controlli va in loop, quindi non seguo i seguenti link
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
		
        // concateno le dir man mano che ispeziono ricorsivamente la struttura del file system locale
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", dir_name, entry->d_name);

		// prendo gli attributi di un file e li inserisco nel buffer st
		// verificando eventuali errori 
        struct stat st;
        if (stat(path, &st) == -1)
            continue;
        

		// vedo se un file è una DIR e nel caso ispeziono ricorsivamente tale dir
        if (S_ISDIR(st.st_mode))	
            search_in_directory(path, target_name, args, env);
        
		
		/* S_ISREG(st.st_mode) verifica se un file è un file regolare (cioè un file 
		non speciale come una directory, un link simbolico, un socket, ecc.) */
		else if (S_ISREG(st.st_mode) && strcmp(entry->d_name, target_name) == 0) {
            
            // provo l'accesso in esecuzione sul file
            if (access(path, X_OK) == 0) {

                pid_t pid;
                switch (pid = fork())
                {
                case 0:

                    char cmd[PATH_MAX];
                    if (chdir(env) == -1)
                        _exit(EXIT_FAILURE);

                    snprintf(cmd, PATH_MAX, "./%s", entry->d_name); 
                    if(execvp(cmd, args) == -1)
                        _exit(EXIT_FAILURE);
                    
                    break;
                
                case -1:
                    perror("Errore nella fork");
                    break;
                
                default:
                    int status;
                    waitpid(pid, &status, 0); //aspetta la fine
                    break;
                }
               
            }   
        }
    }
    
    closedir(dir);
}

int main(int argc, char *argv[]) {
    
    for (size_t i = 0; i < 3; i++)
    {
        printf("OUT %d:\n", i);
        search_in_directory(".", argv[1], argv+1, DIRS_EXECVE[i]);
    }
    
    
    return EXIT_SUCCESS;
}
