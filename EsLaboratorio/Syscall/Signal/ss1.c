#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>



void sigh(int signo) {
	write(STDOUT_FILENO, "SIG\n", 4);
}

int main(int argc, char * argv[]) {
	__uint8_t a = 0;
	signal(SIGUSR1, sigh);
	
	for (;;) {
		sleep(1);
		printf("waiting %d sec \n", a++);
	}
}
