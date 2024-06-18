#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {

    char *newargv[] = {"ls", "/Users/andrea/Desktop", NULL};

    // Esegue il programma specificato dalla path
    // execve("/usr/bin/ls", newargv, NULL);    // per linux

    execve("/bin/ls", newargv, NULL);   // per MACOS
	//execlp("lllls", "ls", "/", NULL);
	//printf("errore %s\n", strerror(errno));
	//perror("exevlp");
}