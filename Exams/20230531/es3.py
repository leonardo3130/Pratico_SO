"""
Scrivere un programma python o uno script bash che cerchi all'interno di un sottoalbero se ci sono
link simbolici che indicano lo stesso file. (hint controllare se coincide il numero dell'i-node del file
indicato)
"""

import os
import sys


def multiple_links(d):
    for v in d.values():
        if v > 1:
            return True
    return False


def find_links(dir, d):
    for root, dirs, files in os.walk(dir):
        for name in files:
            file_path = os.path.join(root, name)
            inode = os.stat(file_path).st_ino
            if inode not in d.keys():
                d[inode] = 0

        for name in files:
            file_path = os.path.join(root, name)
            if os.path.islink(file_path):
                # devo aggiungere root + "/" perché readlink ritorna un path
                # calcolato in relazione alla posizione del link
                inode = os.stat(root + "/" + os.readlink(file_path)).st_ino
                if inode in d.keys():
                    d[inode] += 1
                else:
                    d[inode] = 1  # file puntato non è nel subtree


if __name__ == "__main__":
    dir = sys.argv[1]
    d = {}
    find_links(dir, d)
    print("multiple links") if multiple_links(d) else print("no multiple links")
