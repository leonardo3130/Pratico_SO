
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


#define SIGHUP  1       /* hangup */
#define SIGINT  2       /* interrupt */
#define SIGQUIT 3       /* quit */
#define SIGILL  4       /* illegal instruction (not reset when caught) */
#define SIGTRAP 5       /* trace trap (not reset when caught) */
#define SIGABRT 6       /* abort() */
#if  (defined(_POSIX_C_SOURCE) && !defined(_DARWIN_C_SOURCE))
#define SIGPOLL 7       /* pollable event ([XSR] generated, not supported) */
#else   /* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
#define SIGIOT  SIGABRT /* compatibility */
#define SIGEMT  7       /* EMT instruction */
#endif  /* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
#define SIGFPE  8       /* floating point exception */
#define SIGKILL 9       /* kill (cannot be caught or ignored) */
#define SIGBUS  10      /* bus error */
#define SIGSEGV 11      /* segmentation violation */
#define SIGSYS  12      /* bad argument to system call */
#define SIGPIPE 13      /* write on a pipe with no one to read it */
#define SIGALRM 14      /* alarm clock */
#define SIGTERM 15      /* software termination signal from kill */
#define SIGURG  16      /* urgent condition on IO channel */
#define SIGSTOP 17      /* sendable stop signal not from tty */
#define SIGTSTP 18      /* stop signal from tty */
#define SIGCONT 19      /* continue a stopped process */
#define SIGCHLD 20      /* to parent on child stop or exit */
#define SIGTTIN 21      /* to readers pgrp upon background tty read */
#define SIGTTOU 22      /* like TTIN for output if (tp->t_local&LTOSTOP) */
#if  (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
#define SIGIO   23      /* input/output possible signal */
#endif
#define SIGXCPU 24      /* exceeded CPU time limit */
#define SIGXFSZ 25      /* exceeded file size limit */
#define SIGVTALRM 26    /* virtual time alarm */
#define SIGPROF 27      /* profiling time alarm */
#if  (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
#define SIGWINCH 28     /* window size changes */
#define SIGINFO 29      /* information request */
#endif
#define SIGUSR1 30      /* user defined signal 1 */
#define SIGUSR2 31      /* user defined signal 2 */