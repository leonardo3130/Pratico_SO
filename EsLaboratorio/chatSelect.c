#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>
#define BUFSIZE 1024

/*
 * main: Programma che legge da due file descriptor dati in input e
 *       scrive sullo standard output.
 *
 * Parametri:
 * argc: numero di parametri passati alla funzione
 * argv: array di stringhe contenenti i parametri passati alla funzione
 *
 * Restituisce: 0 se la funzione termina correttamente, altrimenti 1
 */
int main(int argc, char *argv[]) {
	// Apriamo in modalità non bloccante il file descriptor di input
	int fdin = open(argv[1], O_RDONLY | O_NONBLOCK);
	// Apriamo in modalità di scrittura il file descriptor di output
	int fdout = open(argv[2], O_WRONLY);
	// Definiamo il buffer di lettura e scritt da 1024 byte
	char buf[BUFSIZE];

	// Eseguiamo un loop infinito
	for (;;) {
		// Inizializziamo il set di file descriptor da monitorare
		fd_set rfds;
		FD_ZERO(&rfds);
		// Aggiungiamo il file descriptor di input e quello di standard input al set
		FD_SET(STDIN_FILENO, &rfds);
		FD_SET(fdin, &rfds);

		// Selezioniamo il file descriptor con la funzione select (monitoriamo fa 0 a fdin (incluso))
		int srv = select(fdin+1, &rfds, NULL, NULL, NULL);
		// Se è disponibile il file descriptor di standard input
		if (FD_ISSET(STDIN_FILENO, &rfds)) {
			// Leggiamo dallo standard input il numero di byte letti in n
			ssize_t n = read(STDIN_FILENO, buf, BUFSIZE);
			// Se non abbiamo letto nessun byte usciamo dalla funzione
			if (n <= 0)
				return EXIT_FAILURE;
			// Scriviamo sul file descriptor di output il numero di byte letti
			write (fdout, buf, n);
		}
		// Se è disponibile il file descriptor di input
		if (FD_ISSET(fdin, &rfds)) {
			// Leggiamo dal file descriptor di input il numero di byte letti in n
			ssize_t n = read(fdin, buf, BUFSIZE);
			// Se non abbiamo letto nessun byte esciamo dalla funzione
			if (n <= 0)
				return 0;
			// Scriviamo sullo standard output il numero di byte letti
			write (STDOUT_FILENO, buf, n);
		}
	}
}
