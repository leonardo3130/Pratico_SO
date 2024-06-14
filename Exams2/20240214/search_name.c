#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

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
void search_in_directory(const char *dir_name, const char *target_name) {
    DIR *dir = opendir(dir_name);
    if (!dir) {
        perror("opendir");
        return;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
		
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);

		// prendo gli attributi di un file e li inserisco nel buffer st
		// verificando eventuali errori 
        struct stat st;
        if (stat(path, &st) == -1)
            continue;
        

		// vedo se un file è una DIR e nel caso ispeziono ricorsivamente tale dir
        if (S_ISDIR(st.st_mode)) {	
            search_in_directory(path, target_name);
        } 
		
		/* S_ISREG(st.st_mode) verifica se un file è un file regolare (cioè un file 
		non speciale come una directory, un link simbolico, un socket, ecc.) */
		else if (S_ISREG(st.st_mode) && strcmp(entry->d_name, target_name) == 0) {
            if (access(path, X_OK) == 0) {
                
				if (is_script(path)) 
                    printf("%s: script\n", path);

                else if (is_elf(path)) 
                    printf("%s: ELF executable\n", path);
					
                else 
                    printf("%s: unknown executable type\n", path);
                
            }
        }
    }
    
    closedir(dir);
}

int main(int argc, char *argv[]) {
    
    search_in_directory(".", argv[1]);

    return 0;
}
