#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
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

/*
 * Funzione principale che crea un dispositivo speciale.
 */  
int main(int argc, char *argv[]) {
    /*
        S_IFCHR | 0666: Crea un file di dispositivo carattere (S_IFCHR) 
        con permessi di lettura e scrittura per tutti gli utenti (0666)

        makedev(1, 9): Crea un dispositivo con major number 1 e minor number 9
    */
	int rv;
	if ((rv = mknod(argv[1], S_IFCHR | 0666, makedev(1, 9))) < 0)
		perror("mknod");
}
