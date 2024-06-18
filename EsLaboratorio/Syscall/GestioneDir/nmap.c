#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	struct stat st;
	stat(argv[1], &st);     // prendi le informazioni sul file e mettile in st
	printf("file %s length %d\n", argv[1], (int) st.st_size); 

	int fd = open(argv[1], O_RDONLY);   // apri il file in sola lettura

    /*
        Hint Address: Se si passa un indirizzo non NULL e si usa il flag MAP_FIXED, 
        il kernel è obbligato a mappare alla posizione specificata, a meno che non ci sia un errore. 
        Senza MAP_FIXED, l'indirizzo viene considerato un suggerimento, e il kernel può scegliere 
        un altro indirizzo se necessario.

        MAP_SHARED condivide la mappatura con altri processi che mappano lo stesso file.
    */
	char *mmfd = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);  // mappa il file in memoria

	printf("%p\n", mmfd);   // stampa l'indirizzo di memoria in cui è mappato il file

	close(fd);  // chiudi il file

	write(STDOUT_FILENO, mmfd, st.st_size); // scrivi il contenuto del file su stdout

	printf("\n\n%c %c %c\n", mmfd[0], mmfd[1],mmfd[2]); // stampa i primi tre caratteri del file
	
    munmap(mmfd, st.st_size);   // rimuovi la mappatura
}