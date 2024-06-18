#include <stdio.h>

int main(int argc, char * argv[]) {
	FILE *f = fopen(argv[1], "r");  // apre file in lettura
	int a, b, rv, len = 6;
	char s[100];

    

	while ((rv = fscanf(f, "%s %d %d\n", s, &a, &b)) == 3) 
		printf("%-10.10s %*d %05d\n", s, len++, a, b);

    /*
        La stringa di formattazione "%-10.10s %5d %05d\n" specifica il formato di stampa dei dati.
        Nel dettaglio, 
            %s indica che verrà stampata una stringa, 
                - => stringa allineata a sinistra
                10.10 => stringa di 10 caratteri, troncata a 10 caratteri se più lunga
            %d indica che verrà stampato un numero intero, 

            %*d, la funzione `printf` si aspetta due argomenti: 
            il primo argomento è un intero che specifica la larghezza del campo di stampa, 
            mentre il secondo argomento è l'intero effettivo da stampare. 
            
            %5d specifica che il numero intero verrà stampato su 5 caratteri, allineato a destra,
            %05d specifica che il numero intero verrà stampato su 5 caratteri, con zeri iniziali 
                se necessario.
    */
	
    fclose(f);
}

/*  Output:

┌──[andrea  MBP-di-Andrea]—[~/…/Pratico_SO/EsLaboratorio/Syscall/GestioneDir] 
└─$ ./openFile file.txt                                                                                                     
ffffffffff     10 00010
ffffffffff      10 00010
ffffffffff       10 00010
*/