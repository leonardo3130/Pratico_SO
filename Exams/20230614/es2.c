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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <time.h>

#define MAX_EVENTS 10

struct timer_struct {
  int fd;
  int expired;
  int count;
  char s[100];
};

int checkOver(struct timer_struct *timers, int size) {
  for (int i = 0; i < size; i++) {
    if((timers + i)->expired < (timers + i)->count) 
      return 0;
  }
  return 1;
}

void timed_print(char *argv[], int argc) {
  const int SIZE_TIMERS = argc / 3;
  int epollfd, num_events;
  struct epoll_event ev, events[MAX_EVENTS];
  struct timer_struct timers[SIZE_TIMERS];
  uint64_t exp;
  // Creazione di un epollfd per monitorare il timerfd
  if ((epollfd = epoll_create1(0)) == -1) { // creazione istanza di epoll
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  // Creazione dei timer --> ne ho molteplici
  for(int i = 0; i < SIZE_TIMERS; i++) { 
    if ((timers[i].fd = timerfd_create(CLOCK_MONOTONIC, 0)) == -1) {
        perror("timerfd_create");
        exit(EXIT_FAILURE);
    }

    //3*(i+1) - 1 --> stringa
    //3*(i+1) - 2 --> tempo 
    //3*(i+1) - 3 --> count
    timers[i].expired = 0;
    timers[i].count = atoi(argv[3 * (i + 1) - 3]);
    strncpy(timers[i].s, argv[3 * (i + 1) - 1], 100);

    printf("str %s\n", timers[i].s);
    printf("count %d\n", timers[i].count);
    printf("expired %d\n", timers[i].expired);
    // Impostazione del timer a scadenza ogni secondo
    float time = atof(argv[3 * (i + 1) - 2]);
    int seconds = (int)time;
    float remaining = time - (float)seconds;
    remaining *= (float)pow((double)10, (double)9);
    int nseconds = (int)remaining;

    struct itimerspec itval;
    itval.it_interval.tv_sec = seconds;
    itval.it_interval.tv_nsec = nseconds;
    itval.it_value.tv_sec = seconds;
    itval.it_value.tv_nsec = nseconds;
    
    if (timerfd_settime(timers[i].fd, 0, &itval, NULL) == -1) { //inizia o stoppa il timer 
      perror("timerfd_settime");
      exit(EXIT_FAILURE);
    }

    
    //aggiunta eventi all'epoll 
    ev.events = EPOLLIN; //file associato è disponibile per operazioni di lettura
    ev.data.fd = timers[i].fd; //descrittore del file da monitorare
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, timers[i].fd, &ev) == -1) { //EPOLL_CTL_ADD -->  
      perror("epoll_ctl");
      exit(EXIT_FAILURE);
    }
  }
  
  //loop d'ascolto degli eventi
  int count = 0;
  int flag = 0;
  while(flag != 1) {
    //ottengo gli eventi 
    num_events = epoll_wait(epollfd, events, MAX_EVENTS, -1); //-1 parametro timeout --> epoll_wait bloccata in attesa di eventi 
    if (num_events == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }


    for (int i = 0; i < num_events; i++) {
      for(int j = 0; j < SIZE_TIMERS; j++) {
        // Lettura del numero di scadenze del timer
        if(timers[j].fd == events[i].data.fd && timers[j].expired < timers[j].count) {
          //per leggere mi assicuro prima che l'evento sia relativo al fd del timer attuale (j) e che 
          //quest'ultimo non abbia finito le sue stampe
          if (read(events[i].data.fd, &exp, sizeof(uint64_t)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
          }
          timers[j].expired += exp; //exp è il numeo di volte che il timer è scaduto, se > 1 ci sono stati ritardi
          printf("%s\n", timers[j].s);
        }
        flag = checkOver(timers, SIZE_TIMERS);
      }
    }
  }

  //chiusura fd
  for (int i = 0; i < SIZE_TIMERS; i++) 
    close(timers[i].fd);
  close(epollfd);
}

int main(int argc, char *argv[])
{
  timed_print(argv + 1, argc - 1);  
  return EXIT_SUCCESS;
}
