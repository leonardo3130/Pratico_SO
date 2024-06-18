#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

char **vreaddir(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);
    if (dp == NULL) {
        perror("opendir");
        return NULL;
    }

    // Conteggiare il numero di file nella directory
    size_t count = 0;
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_DIR) {
            count++;
        }
    }
    rewinddir(dp);

    // Allocare un unico blocco di memoria per il vettore e le stringhe
    size_t total_size = (count + 1) * sizeof(char*); // per i puntatori alle stringhe
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_DIR) {
            total_size += strlen(entry->d_name) + 1;
        }
    }

    char **names = (char **)malloc(total_size);
    if (names == NULL) {
        perror("malloc");
        closedir(dp);
        return NULL;
    }

    // Assegnare i puntatori alle stringhe
    char *string_space = (char *)(names + count + 1);
    rewinddir(dp);
    size_t i = 0;
    while ((entry = readdir(dp)) != NULL) {
        names[i++] = string_space;
        strcpy(string_space, entry->d_name);
        string_space += strlen(entry->d_name) + 1;
    }
    names[i] = NULL; // Terminare con NULL

    closedir(dp);
    return names;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    char **names = vreaddir(argv[1]);


    for (size_t i = 0; names[i] != NULL; i++) {
        printf("%s\n", names[i]);
    }

    free(names);
    return 0;
}
