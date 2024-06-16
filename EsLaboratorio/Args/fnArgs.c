#include <stdio.h>
#include <stdarg.h>

/*
 * Stampa i numeri passati come argomenti.
 *
 * @param nr Il numero di argomenti passati.
 * @param ... Gli argomenti da stampare.
 */
void pn (int nr, ...) {
    va_list va;         // va_list è un tipo definito in stdarg.h per gestire gli argomenti
    va_start(va, nr);   // inizializza la lista degli argomenti in modo che scorra tutti gli nr args
   
    for (int i = 0; i < nr; i++)
        // va_arg restituisce l'argomento corrente e fa avanzare la lista automaticamente
        printf("%d ", va_arg(va, int));     
    
    printf("\n");
    va_end(va);        // termina la lista degli argomenti
}

int main(int argc, char *argv[]) {
    pn(3,   1,  2,  3);
    pn(2,   21, 22);
    pn(0);
}