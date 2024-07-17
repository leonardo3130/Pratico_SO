
           Name             Default Action       Description
     1     SIGHUP           terminate process    terminal line hangup
     2     SIGINT           terminate process    interrupt program
     3     SIGQUIT          create core image    quit program
     4     SIGILL           create core image    illegal instruction
     5     SIGTRAP          create core image    trace trap
     6     SIGABRT          create core image    abort program (formerly
                                                 SIGIOT)
     7     SIGEMT           create core image    emulate instruction executed
     8     SIGFPE           create core image    floating-point exception
     9     SIGKILL          terminate process    kill program
     10    SIGBUS           create core image    bus error
     11    SIGSEGV          create core image    segmentation violation
     12    SIGSYS           create core image    non-existent system call
                                                 invoked
     13    SIGPIPE          terminate process    write on a pipe with no reader
     14    SIGALRM          terminate process    real-time timer expired
     15    SIGTERM          terminate process    software termination signal
     16    SIGURG           discard signal       urgent condition present on
                                                 socket
     17    SIGSTOP          stop process         stop (cannot be caught or
                                                 ignored)
     18    SIGTSTP          stop process         stop signal generated from
                                                 keyboard
     19    SIGCONT          discard signal       continue after stop
     20    SIGCHLD          discard signal       child status has changed
     21    SIGTTIN          stop process         background read attempted
                                                 from control terminal
     22    SIGTTOU          stop process         background write attempted to
                                                 control terminal
     23    SIGIO            discard signal       I/O is possible on a
                                                 descriptor (see fcntl(2))
     24    SIGXCPU          terminate process    cpu time limit exceeded (see
                                                 setrlimit(2))
     25    SIGXFSZ          terminate process    file size limit exceeded (see
                                                 setrlimit(2))
     26    SIGVTALRM        terminate process    virtual time alarm (see
                                                 setitimer(2))
     27    SIGPROF          terminate process    profiling timer alarm (see
                                                 setitimer(2))
     28    SIGWINCH         discard signal       Window size change
     29    SIGINFO          discard signal       status request from keyboard
     30    SIGUSR1          terminate process    User defined signal 1
     31    SIGUSR2          terminate process    User defined signal 2


man 7 signal-safety:
La pagina del manuale `man 7 signal-safety` tratta delle funzioni di libreria sicure da usare all'interno dei gestori di segnali asincroni in Linux. Queste funzioni sono definite "async-signal-safe" perché possono essere chiamate in modo sicuro da un gestore di segnali senza rischio di comportamenti indefiniti o corruzione di dati.

Ecco un riassunto delle principali sezioni:

1. **Descrizione Generale**: Spiega cosa si intende per funzioni async-signal-safe, che sono funzioni che possono essere utilizzate all'interno dei gestori di segnali senza causare problemi di concorrenza o deadlock. 

2. **Lista delle Funzioni Sicure**: Include una lista di funzioni di libreria standard considerate sicure per l'uso nei gestori di segnali. Tra queste ci sono funzioni come `abort()`, `accept()`, `access()`, `chdir()`, `close()`, `dup()`, `execve()`, `fork()`, `getpid()`, `kill()`, `pause()`, `read()`, `write()`, `sigaction()`, e molte altre.

3. **Funzioni Non Sicure**: Sottolinea che la maggior parte delle altre funzioni di libreria standard non sono sicure per l'uso all'interno dei gestori di segnali e dovrebbero essere evitate. Ad esempio, funzioni che utilizzano lock interni, come molte funzioni di input/output e di manipolazione delle stringhe, non sono async-signal-safe.

4. **Raccomandazioni**: Consiglia l'uso di meccanismi di comunicazione sicura, come pipe, per segnalare l'arrivo di un segnale ad un thread principale che poi eseguirà le azioni appropriate.

Questo argomento è cruciale per sviluppatori che scrivono programmi di sistema o applicazioni che devono gestire segnali asincroni, poiché l'uso di funzioni non sicure in tali contesti può portare a comportamenti imprevedibili e difficili da diagnosticare【9†source】【10†source】【11†source】.

signal VS sigaction vs signalfd
In Linux, `signal`, `sigaction`, e `signalfd` sono tre metodi utilizzati per gestire i segnali. Ognuno di essi ha caratteristiche e modalità d'uso specifiche che li rendono adatti a diverse situazioni. Ecco una descrizione dettagliata delle differenze tra questi tre metodi:

### `signal`

#### Descrizione:
- `signal` è la funzione più semplice per gestire i segnali. Consente di impostare un gestore di segnali per un determinato segnale.

#### Uso:
- Prototipo: `void (*signal(int signum, void (*handler)(int)))(int);`
- `signum` è il numero del segnale da gestire.
- `handler` è la funzione gestore che verrà chiamata quando il segnale è ricevuto. Può essere `SIG_DFL` (comportamento predefinito) o `SIG_IGN` (ignorare il segnale).

#### Limitazioni:
- È meno flessibile rispetto a `sigaction`.
- Non garantisce il comportamento in ambienti multithreading.
- Non permette di ottenere informazioni dettagliate sul segnale ricevuto.

#### Vantaggi:
- Semplicità di utilizzo.
- Adatto per semplici gestioni di segnali nei programmi monothread.

### `sigaction`

#### Descrizione:
- `sigaction` è un'interfaccia più avanzata e flessibile per la gestione dei segnali rispetto a `signal`.

#### Uso:
- Prototipo: `int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);`
- `signum` è il numero del segnale da gestire.
- `act` è un puntatore a una struttura `sigaction` che specifica il nuovo gestore e le opzioni.
- `oldact` è un puntatore a una struttura `sigaction` che memorizza il precedente gestore e le opzioni (può essere NULL se non interessa).

#### Struttura `sigaction`:
- Include campi come `sa_handler` (o `sa_sigaction`), `sa_mask`, `sa_flags`, e `sa_restorer`.
- Permette di specificare opzioni avanzate come `SA_SIGINFO` per ottenere informazioni dettagliate sul segnale e `SA_RESTART` per il riavvio delle chiamate di sistema interrotte.

#### Vantaggi:
- Maggiore controllo e flessibilità.
- Sicurezza in ambienti multithreading.
- Possibilità di ottenere informazioni dettagliate sui segnali.

#### Limitazioni:
- Maggiore complessità rispetto a `signal`.

### `signalfd`

#### Descrizione:
- `signalfd` permette di ricevere segnali come se fossero messaggi su un file descriptor, integrandosi bene con il modello di programmazione basato su I/O asincrono.

#### Uso:
- Prototipo: `int signalfd(int fd, const sigset_t *mask, int flags);`
- `fd` è il file descriptor esistente (o -1 per crearne uno nuovo).
- `mask` è una maschera di segnali che specifica i segnali da catturare.
- `flags` possono includere opzioni come `SFD_NONBLOCK` e `SFD_CLOEXEC`.

#### Vantaggi:
- Integrabile con `select`, `poll`, e `epoll`, facilitando la gestione di segnali in loop di eventi.
- Riceve segnali come dati leggibili da un file descriptor.
- Adatto per applicazioni server e ambienti altamente concorrenti.

#### Limitazioni:
- Richiede una conoscenza del modello di programmazione basato su I/O asincrono.
- Non sempre necessario per semplici gestioni di segnali.

### Confronto

| Caratteristica      | `signal`                 | `sigaction`              | `signalfd`                |
|---------------------|--------------------------|--------------------------|---------------------------|
| Complessità         | Bassa                    | Media                    | Alta                      |
| Flessibilità        | Bassa                    | Alta                     | Media                     |
| Compatibilità       | Monothread               | Multithread              | Multithread               |
| Informazioni        | Limitate                 | Dettagliate (con `SA_SIGINFO`) | Moderate                |
| Modello di I/O      | Non applicabile          | Non applicabile          | Asincrono                 |
| Uso tipico          | Semplici gestioni di segnali | Gestione avanzata dei segnali | Integrazione con loop di eventi |

In sintesi, `signal` è adatto per semplici esigenze di gestione dei segnali, `sigaction` offre maggiore controllo e flessibilità, mentre `signalfd` è ideale per integrazioni con modelli di programmazione basati su I/O asincrono.