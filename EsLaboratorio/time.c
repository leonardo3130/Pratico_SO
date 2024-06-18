#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	time_t now = time(NULL);
	printf("%lld\n", now);			// stampa il tempo attuale in secondi dal 1 gennaio 1970
	printf("%s\n", ctime(&now));	// stampa il tempo attuale in formato stringa
}
