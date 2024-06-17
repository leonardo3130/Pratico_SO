#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	close(STDOUT_FILENO);
	dup(fd);
	
    execvp(argv[2], argv+2);
}

/*
    ./dupExec dupExec.txt ls -l     

    produce il file dupExec.txt con il contenuto della directory corrente (preso da ls -l)

*/