#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	pid_t pid;
    int status, fd;
	switch (pid = fork()) {

        // qui verrà rimpiazzato il terminale per l'output con il file "ciao"
		case 0:
			fd = open("ciao", O_WRONLY | O_CREAT | O_TRUNC, 0666);
			dup2(fd, STDOUT_FILENO);    // se il secondo parametro è un FD aperto, viene chiuso prima di duplicare il primo parametro
			close(fd);
			execlp("ls", "ls", 0);
			_exit(1);

        // qui NON verrà rimpiazzato il terminale
		default:
			wait(&status);
			printf("finito\n");
			break;
		case -1:
			perror("errfork");
	}
}