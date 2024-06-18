#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/time.h>
#include <stdint.h>


int main(int argc, char *argv[]) {


    // Parse the input arguments
    char *arg = argv[1];

    // prendo le iterazioni
    char *token = strtok(arg, ",");
    int num_iterations = atoi(token);

    // prendo l'intervallo in secondi (1.1)
    token = strtok(NULL, ",");
    double interval_seconds = atof(token);

    // prendo il messaggio ("ciao")
    token = strtok(NULL, ",");
    char *message = token;

    // creo un orologio monotonico al fine di misurare intervalli di tempo
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);


    // Configure the timer
    struct itimerspec timer_spec;
    timer_spec.it_value.tv_sec = (int)interval_seconds;
    timer_spec.it_value.tv_nsec = (interval_seconds - (int)interval_seconds) * 1e9;
    timer_spec.it_interval.tv_sec = (int)interval_seconds;
    timer_spec.it_interval.tv_nsec = (interval_seconds - (int)interval_seconds) * 1e9;

    if (timerfd_settime(timer_fd, 0, &timer_spec, NULL) == -1) {
        perror("timerfd_settime");
        close(timer_fd);
        return EXIT_FAILURE;
    }

    // Read from the timer file descriptor and print the message
    uint64_t expirations;
    for (int i = 0; i < num_iterations; i++) {
        if (read(timer_fd, &expirations, sizeof(expirations)) != sizeof(expirations)) {
            perror("read");
            close(timer_fd);
            return EXIT_FAILURE;
        }

        struct timeval now;
        gettimeofday(&now, NULL);
        double elapsed_time = now.tv_sec + now.tv_usec / 1e6;
        printf("%.6f %s\n", elapsed_time, message);
    }

    close(timer_fd);
    return EXIT_SUCCESS;
}
