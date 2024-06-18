#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <libgen.h>

/*
Il codice selezionato è un programma scritto in linguaggio C che accetta 
opzioni da riga di comando e calcola un valore intero in base alle opzioni fornite.

Il programma utilizza la libreria <getopt> per analizzare le opzioni e i relativi argomenti 
passati da riga di comando.

*/

/**
 * Stampa un messaggio di utilizzo e termina il programma.
 *
 * @param progname Il nome del programma.
 */
void usage_and_exit(char *progname) {
    fprintf(stderr,
            "Usage:\n"
            "  %s OPTIONS [value] ... [value]\n"
            "Options:\n"
            "  -v | --verbose:      modalità verbosa\n"
            "  -h | --help:         stampa un breve messaggio di aiuto sull'utilizzo\n"
            "  -a val | --add val   aggiunge un valore\n"
            "  -s val | --sub val   sottrae un valore\n"
            "  --double             raddoppia il risultato finale\n"
            "Questo programma calcola un valore intero\n", progname);
    exit(1);
}

/**
 * Funzione principale.
 *
 * @param argc Il numero di argomenti passati da riga di comando.
 * @param argv Gli argomenti passati da riga di comando.
 * @return 0 in caso di successo, un valore diverso da zero in caso di errore.
 */
int main(int argc, char *argv[]) {


    /*
        contiene le opzioni lunghe supportate dal programma. 
        Ogni opzione è definita con un nome, un tipo di argomento 
        richiesto e un valore intero associato.
    */
	static struct option long_options[] = {
		{"verbose", no_argument, 0, 'v'},
		{"help",    no_argument, 0, 'h'},
		{"add",     required_argument, 0, 'a'},
		{"sub",     required_argument, 0, 's'},
		{"double",  no_argument, 0, 0x101},
		{0,         0,           0, 0}
	};


	static char *short_options = "vha:s:";

	int opt;

	int value = 0;
	int verbose = 0;
	int doubleval = 1;

	char *progname = basename(argv[0]);


    /*
    utilizza la funzione getopt_long per analizzare le opzioni passate da riga di comando. 
    Ogni opzione viene gestita all'interno del blocco switch. Ad esempio, se l'opzione è -v, 
    viene impostata la variabile verbose su 1. Se l'opzione è -a, viene aggiunto il valore 
    dell'argomento convertito in intero alla variabile value. Se l'opzione è --double, 
    viene moltiplicato il valore di doubleval per 2.
    */
	while((opt = getopt_long(argc, argv, short_options, long_options, NULL)) > 0) {
		switch (opt) {
			case 'v': verbose = 1;
								break;
			case 'a': value += atoi(optarg);
								break;
			case 's': value -= atoi(optarg);
								break;
			case 0x101: doubleval *= 2;
									break;
			case 'h':
			case '?': usage_and_exit(progname);
								break;
		}
	}

	for (; optind < argc; optind++)
		value += atoi(argv[optind]);

	value *= doubleval;

	if (verbose)
		printf("the result is %d\n", value);

	return value;
}