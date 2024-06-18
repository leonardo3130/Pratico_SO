#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/**
 * Funzione di gestione del segnale SIGUSR1.
 * Stampa una stringa ricevuta come segnale.
 * 
 * @param signo il numero del segnale ricevuto
 * @param info informazioni aggiuntive sul segnale
 * @param context contesto di esecuzione del segnale
 */
void received_string(int signo, siginfo_t *info, void *context){
    long msg_long = (long)info->si_value.sival_ptr;
    if(msg_long == 0)
        printf("\n");
    else{
        char msg;
        for(int i=0, last = 0; i<8; i++){
            last = msg_long%256;
            msg_long = msg_long>>8;
            printf("%c", last);
        }
    }
    union sigval v;
    sigqueue(info->si_pid, SIGUSR1, v);
}

/**
 * Funzione principale del programma.
 * Stampa il PID del processo e imposta la gestione del segnale SIGUSR1.
 * 
 * @param argc il numero di argomenti passati al programma
 * @param argv array di stringhe contenente gli argomenti passati al programma
 * @return 0 se il programma termina correttamente, altrimenti un valore diverso da 0
 */
int main(int argc, char *argv[]){
    printf("%d\n", getpid());
    struct sigaction act;
    act.sa_sigaction = received_string;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    
    while (1) 
        pause(); // Aspetta un segnale
    
    return 0;
}