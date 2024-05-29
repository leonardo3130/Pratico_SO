#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void sigusr1_handler(int signum) {
    printf("Received SIGUSR1 signal (%d)\n", signum);
    // Add your custom actions here
}

int main(int argc, char *argv[]) {
    //custom handler for SIGUSR1
    if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    // Apertura della named pipe per la scrittura
    int fd = open(argv[1], O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Scrivi il PID e il numero di segnale nella named pipe
    pid_t pid = getpid();
    int signal = SIGUSR1;

    char line[512];
    snprintf(line, sizeof(line) - 1, "%d %d %d\n", pid, signal, 5);

    if (write(fd, line, sizeof(line) - 1) == -1) {
        perror("dprintf");
        exit(EXIT_FAILURE);
    }

    // Chiudi la named pipe
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
