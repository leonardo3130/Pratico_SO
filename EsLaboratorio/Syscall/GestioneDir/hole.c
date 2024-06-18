#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    /*
    SEEK_SET: Imposta il puntatore del file a offset byte dall'inizio del file.
    SEEK_CUR: Sposta il puntatore del file di offset byte rispetto alla posizione corrente.
    SEEK_END: Imposta il puntatore del file a offset byte dalla fine del file.
    */

    write(fd, "prova\n", 6);        // Scrive "prova\n" all'inizio del file
    lseek(fd, 8192, SEEK_SET);     // Sposta il puntatore del file a 8192 byte dall'inizio
    write(fd, "fine\n", 5);        // Scrive "fine\n" alla nuova posizione

    close(fd);
    return 0;
}

/*
    Un "hole" in un file è una sezione del file che non ha dati fisicamente memorizzati 
    sul disco. Quando si crea un buco in un file, il sistema operativo non alloca 
    spazio su disco per i byte tra i dati scritti. Al posto di ciò, il filesystem 
    riconosce che queste sezioni sono "buchi" e restituisce byte nulli (valori zero) 
    quando il file è letto.

    Utilità dei Buchi: I buchi possono essere utili per risparmiare spazio su disco 
    quando si lavora con file sparsi (sparse files), dove grandi sezioni del file 
    sono vuote e non è necessario allocare spazio su disco per queste sezioni.

    per vedere i buchi usare "od -c holeTest":
    0000000    p   r   o   v   a  \n  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
    0000020   \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
    *
    0020000    f   i   n   e  \n                                            
    0020005
*/
