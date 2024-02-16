#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	pid_t fork_retvalue;
	switch (fork_retvalue = fork()) {
		case 0:
			execvp(argv[1], argv+1);
			_exit(42); //linea ignorata --> il processo viene totalmente rimpiazzato da quello dell'exec
			break;
		default:
			int status;
			waitpid(fork_retvalue, &status, 0);
			printf("fine wait %d\n", WEXITSTATUS(status));
			break;
		case -1:
			perror("fork");
	}
}
