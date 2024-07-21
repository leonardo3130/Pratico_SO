/*
Estendere l'esercizio 1 in modo che gestisca molteplici timer:
$ mftdtest 2,1.1,a 2,1.2,b 3,0.5,c
0.500255 c
1.000232 c
1.100231 a
1.200266 b
1.500245 c
2.200212 a
2.400255 b
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

struct timer_exp {
  uint64_t times;
  uint64_t expired;
  char *str;
  int tfd;
};

int checkOver(struct timer_exp t_structs[], int ntimers) {
  for (int i = 0; i < ntimers; i++) {
    if (t_structs[i].expired < t_structs[i].times)
      return 1;
  }
  return 0;
}

int main(int argc, char *argv[]) {

  int ntimers = argc - 1;
  int tfds[ntimers];
  char *vals[3 * ntimers];
  int c = 0;
  int l;
  struct timer_exp t_structs[ntimers];
  struct epoll_event events[MAX_EVENTS];
  int efd = epoll_create1(0);
  if (efd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < ntimers; i++) {
    tfds[i] = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tfds[i] == -1) {
      perror("timerfd_create");
      exit(EXIT_FAILURE);
    }

    l = strlen(argv[i + 1]);
    vals[c++] = argv[i + 1];
    for (int j = 0; j < l; j++) {
      if (argv[i + 1][j] == ',') {
        argv[i + 1][j] = '\0';
        vals[c++] = &argv[i + 1][j + 1];
      }
    }

    t_structs[i].times = atoi(vals[i * 3]);
    t_structs[i].expired = 0;
    t_structs[i].tfd = tfds[i];
    t_structs[i].str = vals[i * 3 + 2];
    float time = atof(vals[i * 3 + 1]);
    int s = (int)time;
    float remaining = time - (float)s;
    remaining *= (float)pow((double)10, (double)9);
    int ns = (int)remaining;

    struct itimerspec timer;
    timer.it_value.tv_sec = s;
    timer.it_value.tv_nsec = ns;
    timer.it_interval.tv_sec = s;
    timer.it_interval.tv_nsec = ns;

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = tfds[i];

    if (timerfd_settime(tfds[i], 0, &timer, NULL) == -1) {
      perror("timerfd_settime");
      exit(EXIT_FAILURE);
    }
    if (epoll_ctl(efd, EPOLL_CTL_ADD, tfds[i], &ev) == -1) {
      perror("epoll_ctl");
      exit(EXIT_FAILURE);
    }
  }

  uint64_t exp;
  int flag = 1;
  while (flag) {
    int n = epoll_wait(efd, events, MAX_EVENTS, -1);
    if (n == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < ntimers; j++) {
        if (events[i].data.fd == t_structs[j].tfd &&
            t_structs[j].expired < t_structs[j].times) {
          if (read(events[i].data.fd, &exp, sizeof(uint64_t)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
          }
          t_structs[j].expired += exp;
          printf("\n%s", t_structs[j].str);
          break;
        }
      }
    }
    flag = checkOver(t_structs, ntimers);
  }
  for (int i = 0; i < ntimers; i++)
    close(tfds[i]);
  close(efd);

  return EXIT_SUCCESS;
}
