#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char * argv[]) {
	pid_t pid = strtol(argv[1], NULL, 10);  // converti il primo argomento in un intero
	int signo = strtol(argv[2], NULL, 10);  // converti il secondo argomento in un intero
	kill(pid, signo);   					// invia il segnale al processo con PID uguale a pid
}