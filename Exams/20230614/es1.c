/*
Facendo uso dei timerfd (vedi timerfd_create) scrivere un programma che stampi una stringa a
intervalli regolari. (il parametro ha tre campi separati da virgola: il numero di iterazioni, l'intervallo fra
iterazione e la stringa da salvare:
tfdtest 4,1.1,ciao
deve stampare ciao quattro volte, rispettivamente dopo 1.1 secondi, 2.2 secondi, 3.3 secondi 4.4
secondi e terminare. L'esecuzione dovrebbe essere simile alla seguente:
$ tfdtest 4,1.1,ciao
1.100267 ciao
2.200423 ciao
3.300143 ciao
4.400053 ciao
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

void timed_print(int iter, float time, char *s) {
  int tfd, epollfd, num_events;
  struct epoll_event ev, events[MAX_EVENTS];
  uint64_t exp;

  // Creazione di un timerfd
  if ((tfd = timerfd_create(CLOCK_MONOTONIC, 0)) == -1) {
      perror("timerfd_create");
      exit(EXIT_FAILURE);
  }

  // Impostazione del timer a scadenza ogni secondo
  int seconds = (int)time;
  float remaining = time - (float)seconds;
  remaining *= (float)pow((double)10, (double)9);
  int nseconds = (int)remaining;

  struct itimerspec itval;
  itval.it_interval.tv_sec = seconds;
  itval.it_interval.tv_nsec = nseconds;
  itval.it_value.tv_sec = seconds;
  itval.it_value.tv_nsec = nseconds;
  
  if (timerfd_settime(tfd, 0, &itval, NULL) == -1) { //inizia o stoppa il timer 
    perror("timerfd_settime");
    exit(EXIT_FAILURE);
  }

  // Creazione di un epollfd per monitorare il timerfd
  if ((epollfd = epoll_create1(0)) == -1) { // creazione istanza di epoll
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }
  
  //aggiunta tfd all'epoll set 
  ev.events = EPOLLIN; //file associato è disponibile per operazioni di lettura
  ev.data.fd = tfd; //descrittore del file da monitorare
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, tfd, &ev) == -1) { //EPOLL_CTL_ADD -->  
    perror("epoll_ctl");
    exit(EXIT_FAILURE);
  }
  
  //loop d'ascolto degli eventi
  int count = 0;
  while(count < iter) {
    //ottengo gli eventi 
    num_events = epoll_wait(epollfd, events, MAX_EVENTS, -1); //-1 parametro timeout --> epoll_wait bloccata in attesa di eventi 
    if (num_events == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_events; i++) {
      // Lettura del numero di scadenze del timer
      if (read(events[i].data.fd, &exp, sizeof(uint64_t)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }
      count += exp; //exp è il numeo di volte che il timer è scaduto, se > 1 ci sono stati ritardi
      printf("%s\n", s);
    }
  }

  close(tfd);
  close(epollfd);
}

int main(int argc, char *argv[])
{
  timed_print(atoi(argv[1]), atof(argv[2]), argv[3]);  
  return EXIT_SUCCESS;
}
