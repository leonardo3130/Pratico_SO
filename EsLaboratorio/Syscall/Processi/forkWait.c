#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t fork_retvalue;
	if (fork_retvalue = fork()) {
		int status;
		printf("genitore %d %d\n", getpid(), fork_retvalue);
		wait(&status);
        if (WIFEXITED(status)) {
            // Vedo se il processo è morto "normalmente"
            printf("Figlio morto con status = %d\n", WEXITSTATUS(status));
        } else {
            // Morto senza exit o _exit
            printf("Figlio morto senza segnalare la sua morte\n");
        }

	} else {
		sleep(1);
		printf("figlio %d %d\n", getpid(), getppid());
		_exit(42);
	}
}