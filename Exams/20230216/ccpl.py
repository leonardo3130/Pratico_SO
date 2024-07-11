"""
Sia data una directory che contiene file di testo.
Scopo dell'esercizio e' di scrivere un programma Python o uno script bash chiamato ccpl che conti i
caratteri delle corrispondenti righe di testo di tutti i file della directory, si vuole cioe' sapere il 
numero totale di caratteri presenti nelle prime righe di tutti i file, nelle seconde linee, ecc.
$ ccpl mydir
1 234
2 21
3 333
…..
l'ouput significa che se contiamo tutti i caratteri contenuti nella prima riga di tutti i file in mydir
otteniamo 234 (mydir/file1 puo' avere 40 caratteri nella prima riga, mydir/file2 ne puo' avere 20, ecc...
procedendo per tutti i file di mydir la somma fa 234)
"""

import os
import sys


def count_file_lines(file):
    l = []
    with open(file, "r") as f:
        lines = f.readlines()
        for line in lines:
            l.append(len(line) - 1)
    print(l)
    return l


def count_directory_lines(dir):
    dl = []
    for entry in os.listdir(dir):
        if os.path.isfile(os.path.join(dir, entry)):
            l = count_file_lines(os.path.join(dir, entry))
            for i in range(len(l)):
                if i < len(dl):
                    dl[i] += l[i]
                else:
                    dl.append(l[i])
    return dl


if __name__ == "__main__":
    dir = sys.argv[1]
    print(count_directory_lines(dir))
