#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    /*
        Il codice selezionato apre un file specificato come argomento sulla riga di comando utilizzando la funzione open() 
        della libreria di sistema in C. Vengono usati i flag:
            - O_WRONLY per aprire il file in modalità di scrittura,  
            - O_CREAT per creare il file se non esiste già,
            - O_TRUNC per troncare il file se esiste già.
    */
	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    /*
         chiudere il file descriptor standard di output (STDOUT_FILENO). Questo significa che la funzione printf() non 
         scriverà più sulla console, ma verrà reindirizzata sul file aperto precedentemente.
    */
	close(STDOUT_FILENO);

    /*
        duplicare il file descriptor del file aperto precedentemente (fd) sul file descriptor standard di output (STDOUT_FILENO).
        Questo significa che la funzione printf() scriverà sul file aperto precedentemente.
    */
	dup(fd);

    // stampa sul file aperto precedentemente
	printf("hello world\n");
}