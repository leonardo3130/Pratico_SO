#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	int fd = open(argv[1], O_RDONLY);
	char buf[10];

    /*
        In un processo Unix/Linux, ci sono tre descrittori di file aperti di default:
            0: Standard Input (stdin)
            1: Standard Output (stdout)
            2: Standard Error (stderr)

        Dunque è normale che venga stampato 3 perchè è il primo disponibile 
    */
	printf("fd=%d\n", fd);

	ssize_t n = read(fd, buf, 10);      // legge 10 byte dal file descriptor aperto e li mette nel buffer

	write(STDOUT_FILENO, buf, n);       // stampa n catatteri dal buffer sullo standard output
	write(STDOUT_FILENO, "\n", 1);
}