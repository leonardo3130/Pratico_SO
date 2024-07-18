"""
Scrivere un programma pyhton o uno script bash che calcoli l'ampiezza totale in byte dei file
eseguibili ELF presenti in tutte le directory passate come parametri o nella directory corrente se non
viene specificato alcun parametro.
e.g.
$ elfsize /bin /usr/bin
1682573547
"""

import sys
import os


def isELF(path):
    try:
        with open(path, "rb") as f:
            magic = f.read(4)
            if magic == b"\x7fELF":
                return True
            else:
                return False
    except:
        return False


def getDirELFSize(path):
    sum = 0
    for e in os.listdir(path):
        if isELF(os.path.join(path, e)):
            sum += os.path.getsize(os.path.join(path, e))
    return sum


if __name__ == "__main__":
    sum = 0
    if len(sys.argv) == 1:
        sum = getDirELFSize(".")
    else:
        for i in range(1, len(sys.argv)):
            sum += getDirELFSize(sys.argv[i])
    print(f"Sum of ELF files: {sum}")
