#include <stdio.h>
#include <unistd.h>

int main() {
	pid_t fork_retvalue;
	switch (fork_retvalue = fork())
    {
    case 0:
        printf("figlio %d %d %d\n", getpid(), getppid(), fork_retvalue);
        break;

    case -1:
        perror("fork error");
        break;
    
    default:
        printf("genitore %d %d\n", getpid(), fork_retvalue);
        break;
    } 
		
}