"""
Scrivere un programma/script che faccia la lista riscorsiva dei file in un sottoalbero riportando in
ordine alfabetico per nome di file in quale/quali sottodirectory compare.
e.g.
rlsr mydir
ciao: . ./a
mare: ./a ./b
sole: .
Significato: un file con nome ciao esite nella directory mydir ma anche all'interno della sottodirectory
a (esistono cioe' i file mydir/ciao e mydir/a/ciao)
"""

import os
import sys


def scan_subtree(dir):
    d = {}
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file not in d:
                d[file] = [root]
            else:
                d[file].append(root)
    for k in d.keys():
        d[k].sort()
    return d


if __name__ == "__main__":
    dir = sys.argv[1]
    d = scan_subtree(dir)
    for k, v in d.items():
        print(f"{k}: {v}")
