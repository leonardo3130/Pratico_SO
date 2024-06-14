### Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Facendo uso dei timerfd (vedi timerfd_create) scrivere un programma che stampi una stringa a intervalli regolari. Il parametro ha tre campi separati da virgola: 
    il numero di iterazioni, 
    l'intervallo fra iterazione
    la stringa da salvare:

tfdtest 4,1.1,ciao
deve stampare ciao quattro volte, rispettivamente dopo 1.1 secondi, 2.2 secondi, 3.3 secondi 4.4
secondi e terminare. 

L'esecuzione dovrebbe essere simile alla seguente:
$ tfdtest 4,1.1,ciao
1.100267 ciao
2.200423 ciao
3.300143 ciao
4.400053 ciao

Ecco un approfondimento sulle funzioni citate:

## 1. `timerfd_create`

Questa funzione crea un nuovo timer che notifica le sue scadenze leggendo da un file descriptor.

- **Sintassi**: `int timerfd_create(int clockid, int flags);`
- **Parametri**:
  - `clockid`: Tipo di orologio da utilizzare. Solitamente si usa `CLOCK_REALTIME` o `CLOCK_MONOTONIC`.
  - `flags`: Opzioni per il file descriptor, come `TFD_NONBLOCK` per operazioni non bloccanti.
- **Ritorno**: Restituisce un file descriptor per il nuovo timer, o -1 in caso di errore.

Ecco una breve descrizione dei possibili orologi da utilizzare con `timerfd_create`:

#`CLOCK_REALTIME`
- **Descrizione**: Misura il tempo reale (tempo civile corrente). È influenzato da modifiche manuali all'orologio di sistema e da aggiornamenti tramite NTP.
- **Uso**: Per applicazioni sincronizzate con l'ora di sistema.

#`CLOCK_MONOTONIC`
- **Descrizione**: Misura il tempo monotono dall'avvio del sistema. Non è influenzato da modifiche dell'ora di sistema.
- **Uso**: Per misurare intervalli di tempo in modo continuo.

#`CLOCK_BOOTTIME`
- **Descrizione**: Simile a `CLOCK_MONOTONIC`, ma include il tempo trascorso durante la sospensione del sistema.
- **Uso**: Per applicazioni che devono tener conto del tempo totale di funzionamento del sistema, inclusa la sospensione.

#`CLOCK_MONOTONIC_RAW`
- **Descrizione**: Simile a `CLOCK_MONOTONIC`, ma senza correzioni di frequenza dell'orologio.
- **Uso**: Per misurazioni di tempo di alta precisione non influenzate da aggiustamenti dell'orologio.

## 2. `timerfd_settime`

Questa funzione imposta il tempo iniziale e l'intervallo per il timer creato con `timerfd_create`.

- **Sintassi**: `int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);`
- **Parametri**:
  - `fd`: Il file descriptor del timer.
  - `flags`: Se `TFD_TIMER_ABSTIME` è impostato, `new_value->it_value` è considerato come un tempo assoluto.
  - `new_value`: Struttura `itimerspec` che specifica il tempo iniziale (`it_value`) e l'intervallo (`it_interval`).
  - `old_value`: (Opzionale) Se non NULL, questa struttura verrà riempita con le impostazioni precedenti del timer.
- **Ritorno**: Restituisce 0 in caso di successo, o -1 in caso di errore.

## 3. `read`

La funzione `read` legge dati dal file descriptor. Nel contesto dei timer, blocca l'esecuzione fino alla scadenza del timer e poi restituisce il numero di scadenze del timer.

- **Sintassi**: `ssize_t read(int fd, void *buf, size_t count);`
- **Parametri**:
  - `fd`: Il file descriptor da cui leggere.
  - `buf`: Buffer in cui memorizzare i dati letti.
  - `count`: Numero massimo di byte da leggere.
- **Ritorno**: Restituisce il numero di byte letti, o -1 in caso di errore.

## 4. `gettimeofday`

Questa funzione ottiene l'ora corrente.

- **Sintassi**: `int gettimeofday(struct timeval *tv, struct timezone *tz);`
- **Parametri**:
  - `tv`: Puntatore a una struttura `timeval` che verrà riempita con il tempo corrente.
  - `tz`: (Obsoleto) Puntatore a una struttura `timezone`. Solitamente NULL.
- **Ritorno**: Restituisce 0 in caso di successo, o -1 in caso di errore.

## 5. Struttura `itimerspec`

Questa struttura è utilizzata per specificare le impostazioni del timer.

- **Definizione**:
  ```c
  struct timespec {
      time_t tv_sec;  /* seconds */
      long tv_nsec;   /* nanoseconds */
  };

  struct itimerspec {
      struct timespec it_interval;  /* Timer interval */
      struct timespec it_value;     /* Initial expiration */
  };
  ```
- **Campi**:
  - `it_interval`: L'intervallo tra le scadenze successive.
  - `it_value`: Il tempo iniziale fino alla prima scadenza.

## 6. Struttura `timeval`

Questa struttura è utilizzata per rappresentare il tempo in secondi e microsecondi.

- **Definizione**:
  ```c
  struct timeval {
      time_t tv_sec;      /* seconds */
      suseconds_t tv_usec;/* microseconds */
  };
  ```

## Esempio d'uso nel contesto

- `timerfd_create` crea un timer e restituisce un file descriptor.
- `timerfd_settime` imposta il tempo iniziale e l'intervallo del timer.
- `read` sul file descriptor del timer blocca l'esecuzione fino alla scadenza del timer.
- `gettimeofday` ottiene il tempo corrente per calcolare l'intervallo trascorso.

Queste funzioni permettono di creare un timer preciso e gestito tramite file descriptor, utile in applicazioni che richiedono notifiche temporali regolari.