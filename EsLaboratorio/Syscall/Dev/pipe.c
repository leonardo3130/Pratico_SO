#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFSIZE 1024

/*
 * Questo programma crea una pipe e avvia due processi figli.
 * Il primo processo figlio esegue il comando "sort -r" leggendo l'input dalla pipe.
 * Il secondo processo figlio esegue il comando "ls /" scrivendo l'output nella pipe.
 * Il processo padre attende la terminazione dei due processi figli.
 */

int main(int argc, char *argv[]) {
    
    /*
        fd[2] è un array di due interi che rappresenta i file descriptor della pipe: fd[0] per la lettura e fd[1] 
        per la scrittura.
        
        pipe(fd) crea una pipe. Una pipe è un meccanismo di inter-process communication (IPC) che consente 
        a un processo di inviare dati a un altro processo.
    */
    int fd[2];
    char buf[BUFSIZE];
    pipe(fd);

    switch (fork()) {
        case 0:
            // Codice del primo processo figlio che esegue il comando "sort -r" leggendo l'input dalla pipe
            close(fd[1]);   // chiude il lato di scrittura della pipe
            dup2(fd[0], STDIN_FILENO);  // chiude lo standard input e lo rimpiazza con il lato di lettura della pipe
            close(fd[0]);   // chiude il lato di lettura della pipe
            execlp("sort", "sort", "-r", 0);   // esegue il comando "sort -r"
            exit(1);    // se execlp fallisce, termina il processo figlio
        default:
            break;
        case -1:
            perror("fork");
            exit(1);
    }

    switch (fork()) {
        case 0:
            // Codice del secondo processo figlio che esegue il comando "ls /" e scrive l'output nella pipe
            close(fd[0]);   // chiude il lato di lettura della pipe
            dup2(fd[1], STDOUT_FILENO);   // chiude lo standard output e lo rimpiazza con il lato di scrittura della pipe
            close(fd[1]);   // chiude il lato di scrittura della pipe
            execlp("ls", "ls", "/Users/andrea/Desktop", 0);
            break;
        default:
            break;
        case -1:
            perror("fork");
            exit(1);
    }

    close(fd[0]); close(fd[1]);     // chiude entrambi i lati della pipe
    int status;
    wait(&status); wait(&status);   // attende la terminazione dei due processi figli
    return 0;
}