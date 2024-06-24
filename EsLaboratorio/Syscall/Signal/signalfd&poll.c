#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <poll.h>
#include <sys/signalfd.h>
#include <fcntl.h>

int main() {
    // Configura la maschera dei segnali
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    
    // Blocca i segnali per impedire che vengano gestiti nel modo predefinito
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    // Crea il file descriptor per i segnali
    int sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        perror("signalfd");
        exit(EXIT_FAILURE);
    }

    // Configura la struttura pollfd per monitorare stdin e signalfd
    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = sfd;
    fds[1].events = POLLIN;

    for(;;) {
        int ret = poll(fds, 2, -1);
        if (ret == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        // Controlla se c'è input su stdin
        if (fds[0].revents & POLLIN) {
            char buf[256];
            ssize_t num_read = read(STDIN_FILENO, buf, sizeof(buf) - 1);
            if (num_read == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            buf[num_read] = '\0';
            printf("Input da stdin: %s", buf);
        }

        // Controlla se ci sono segnali da gestire
        if (fds[1].revents & POLLIN) {
            struct signalfd_siginfo si;
            ssize_t num_read = read(sfd, &si, sizeof(si));
            if (num_read != sizeof(si)) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            
            if (si.ssi_signo == SIGINT) {
                printf("Ricevuto SIGINT\n");
            } else if (si.ssi_signo == SIGTERM) {
                printf("Ricevuto SIGTERM\n");
                break; // Esce dal loop per terminare il programma
            }
        }
    }

    // Chiude il file descriptor dei segnali
    close(sfd);
    return 0;
}
