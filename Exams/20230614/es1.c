/*
Facendo uso dei timerfd (vedi timerfd_create) scrivere un programma che stampi
una stringa a intervalli regolari. (il parametro ha tre campi separati da
virgola: il numero di iterazioni, l'intervallo fra iterazione e la stringa da
salvare: tfdtest 4,1.1,ciao deve stampare ciao quattro volte, rispettivamente
dopo 1.1 secondi, 2.2 secondi, 3.3 secondi 4.4 secondi e terminare. L'esecuzione
dovrebbe essere simile alla seguente: $ tfdtest 4,1.1,ciao 1.100267 ciao
2.200423 ciao
3.300143 ciao
4.400053 ciao
*/

#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>

#define MAX 4096
#define MAX_EVENTS 16

int main(int argc, char *argv[]) {

  int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (tfd == -1) {
    perror("timerfd_create");
    exit(EXIT_FAILURE);
  }

  char *vals[3];
  vals[0] = argv[1];
  int c = 1;
  int l = strlen(argv[1]);
  for (int i = 0; i < l; i++) {
    if (argv[1][i] == ',') {
      argv[1][i] = '\0';
      vals[c++] = &argv[1][i + 1];
    }
  }

  int times = atoi(vals[0]);
  float time = atof(vals[1]);
  int s = (int)time;
  float remaining = time - (float)s;
  remaining *= (float)pow((double)10, (double)9);
  int ns = (int)remaining;

  struct itimerspec timer;
  timer.it_value.tv_sec = s;
  timer.it_value.tv_nsec = ns;
  timer.it_interval.tv_sec = s;
  timer.it_interval.tv_nsec = ns;

  if (timerfd_settime(tfd, 0, &timer, NULL) == -1) {
    perror("timerfd_settime");
    exit(EXIT_FAILURE);
  }

  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN;
  ev.data.fd = tfd;

  int efd = epoll_create1(0);
  if (efd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  if (epoll_ctl(efd, EPOLL_CTL_ADD, tfd, &ev) == -1) {
    perror("epoll_ctl");
    exit(EXIT_FAILURE);
  }

  uint64_t exp;
  while (times > 0) {
    int n = epoll_wait(efd, events, MAX_EVENTS, -1);
    if (n == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
      if (read(events[i].data.fd, &exp, sizeof(uint64_t)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }
      // exp = numero di expires dall'ultima lettura o dall'inizio (se non
      // letto)
      printf("%s\n", vals[2]);
      times -= exp;
    }
  }
  close(tfd);
  close(efd);

  return EXIT_SUCCESS;
}
