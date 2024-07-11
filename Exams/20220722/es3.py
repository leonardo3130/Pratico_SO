"""
Il programma python o lo script bash deve eseguire uno dopo l'altro tutti gli script presenti nella
directory passata come parametro (o la current directory se manca il parametro) ma non gli eseguibili
binari di tipo ELF.
"""

import os
import sys
import subprocess


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


def isScript(path):
    try:
        with open(path, "rb") as f:
            magic = f.read(2)
            if magic == b"#!":
                return True
            else:
                return False
    except:
        return False


if __name__ == "__main__":
    dir = sys.argv[1]

    # listidir ritorna una lista con i nomi nella directory
    for filename in os.listdir(dir):
        path = os.path.join(dir, filename)
        if os.path.isfile(path) and (isScript(path) and not isELF(path)):
            subprocess.run(path)
