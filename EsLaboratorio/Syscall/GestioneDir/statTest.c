#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/*
In macOS, le macro major, minor e makedev sono disponibili in sys/types.h e sys/stat.h, ma per 
compatibilità e per assicurarsi che il codice funzioni anche su Linux, è meglio includere sys/sysmacros.h.
*/

/////////////////////////////////////////////////
// #include <sys/sysmacros.h>       // non funziona su macos
#include <sys/types.h>              // non so se funziona anche su Linux
/////////////////////////////////////////////////

#include <time.h>

int main(int argc, char *argv[]) {
    struct stat sb;  // Dichiarazione della struttura stat per memorizzare le informazioni del file

    if (argc != 2) {
        // Se il numero di argomenti non è esattamente 2, stampa un messaggio di errore e termina il programma
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Chiamata a lstat per ottenere le informazioni del file. Se fallisce, stampa un messaggio di errore e termina il programma
    if (lstat(argv[1], &sb) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    // Stampa l'ID del dispositivo contenente il file
    printf("ID of containing device:  [%x,%x]\n",
           major(sb.st_dev),  // Numero major del dispositivo
           minor(sb.st_dev)); // Numero minor del dispositivo

    // Stampa il tipo di file
    printf("File type:                ");
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");            break; // Dispositivo a blocchi
        case S_IFCHR:  printf("character device\n");        break; // Dispositivo a caratteri
        case S_IFDIR:  printf("directory\n");               break; // Directory
        case S_IFIFO:  printf("FIFO/pipe\n");               break; // FIFO o pipe
        case S_IFLNK:  printf("symlink\n");                 break; // Link simbolico
        case S_IFREG:  printf("regular file\n");            break; // File regolare
        case S_IFSOCK: printf("socket\n");                  break; // Socket
        default:       printf("unknown?\n");                break; // Tipo sconosciuto
    }

    // Stampa il numero di inode del file
    printf("I-node number:            %ju\n", (uintmax_t) sb.st_ino);

    // Stampa i permessi del file in formato ottale
    printf("Mode:                     %jo (octal)\n", (uintmax_t) sb.st_mode);

    // Stampa il numero di link al file
    printf("Link count:               %ju\n", (uintmax_t) sb.st_nlink);

    // Stampa l'UID e il GID del proprietario del file
    printf("Ownership:                UID=%ju   GID=%ju\n",
           (uintmax_t) sb.st_uid, (uintmax_t) sb.st_gid);

    // Stampa la dimensione del blocco di I/O preferito
    printf("Preferred I/O block size: %jd bytes\n", (intmax_t) sb.st_blksize);

    // Stampa la dimensione del file in byte
    printf("File size:                %jd bytes\n", (intmax_t) sb.st_size);

    // Stampa il numero di blocchi allocati sul disco per il file
    printf("Blocks allocated:         %jd\n", (intmax_t) sb.st_blocks);

    // Stampa la data e l'ora dell'ultimo cambiamento di stato del file
    printf("Last status change:       %s", ctime(&sb.st_ctime));

    // Stampa la data e l'ora dell'ultimo accesso al file
    printf("Last file access:         %s", ctime(&sb.st_atime));

    // Stampa la data e l'ora dell'ultima modifica del file
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

    exit(EXIT_SUCCESS); // Termina il programma con successo
}

/*
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    struct stat fileStat;
    const char *filePath = "example.txt";

    if (stat(filePath, &fileStat) < 0) {
        perror("stat");
        return 1;
    }

    printf("File: %s\n", filePath);
    printf("Inode: %ld\n", (long)fileStat.st_ino);
    printf("File mode: %o\n", fileStat.st_mode);
    printf("Number of links: %ld\n", (long)fileStat.st_nlink);
    printf("User ID: %ld\n", (long)fileStat.st_uid);
    printf("Group ID: %ld\n", (long)fileStat.st_gid);
    printf("File size: %lld bytes\n", (long long)fileStat.st_size);
    printf("Blocks allocated: %lld\n", (long long)fileStat.st_blocks);
    printf("Last access time: %ld\n", (long)fileStat.st_atime);
    printf("Last modification time: %ld\n", (long)fileStat.st_mtime);
    printf("Last status change time: %ld\n", (long)fileStat.st_ctime);

    return 0;
}


*/