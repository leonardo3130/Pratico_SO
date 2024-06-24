#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig) {
    printf("\nNon voglio morire\n");
    exit(EXIT_SUCCESS);
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    printf("Premi Ctrl+C per terminare il programma...\n");

    // Loop infinito per mantenere il programma in esecuzione
    for(;;) 
        pause();
    
    return EXIT_SUCCESS;
}
