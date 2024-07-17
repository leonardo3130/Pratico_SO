"""
Scrivere uno script bash o un programma python che stampi l'elenco dei file di una directory
catalogati per output del comando 'file'. es:
ASCII text: file.txt
directory: mydir
empty: nullfile
PDF document, version 1.6: uno.pdf prova.pdf
"""

import os
import subprocess
import sys


def checkFiles(dir):
    d = {}
    d["directory"] = []
    d["empty"] = []
    for file in os.listdir(dir):
        file_path = os.path.join(dir, file)
        if os.path.isfile(file_path):
            out = subprocess.run(
                ["file", "-b", file_path],
                capture_output=True,
                text=True,
                check=True,
            )
            if out.stdout.strip() in d.keys():
                d[out.stdout.strip()].append(file_path)
            else:
                d[out.stdout.strip()] = [file_path]
        elif os.path.isdir(file_path):
            d["directory"].append(file_path)

    for k, v in d.items():
        print(f"{k}: {v}")


if __name__ == "__main__":
    dir = sys.argv[1]
    checkFiles(dir)
