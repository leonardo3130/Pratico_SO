"""
Scrivere un programma python o uno script bash che, passata una directory come parametro, cancelli
nel sottoalbero generato dalla directory passata come parametro tutti i link simbolici relativi (e non
cancelli quelli assoluti)
lrwxrwxrwx 1 renzo renzo 13 Jun 11 17:03 hostname1 -> /etc/hostname
lrwxrwxrwx 1 renzo renzo 15 Jun 11 17:04 hostname2 -> ../etc/hostname
il primo va mantenuto e il secondo cancellato
"""

import os
import sys


def deleteRelativeLinks(dir):
    for entry in os.listdir(dir):
        path = os.path.join(dir, entry)
        if os.path.islink(path):
            target_path = os.readlink(path)
            if target_path[0] != "/":
                os.remove(path)


if __name__ == "__main__":
    dir = sys.argv[1]
    deleteRelativeLinks(dir)
