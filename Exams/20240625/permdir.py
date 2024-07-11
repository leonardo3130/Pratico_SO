import os
import subprocess
import sys

"""
script che crea tante directory quanti sono i permessi diversi per i vari file nella directory
passata come parametro e mette link simbolici in queste directory che puntano ai file con i 
permessi corrispondenti alle directory 
"""


def getPermissions(dir):
    p = subprocess.run(["ls", "-l", dir], capture_output=True, text=True, check=True)
    return p.stdout.split("\n")[1:-1]


def createDirs(dir, p):
    d = {}

    for permission in p:
        mask = permission.split(" ")[0]
        file_name = permission.split(" ")[-1]
        if mask not in d.keys():
            if mask[0] == "-":  # solo file, no dir, no file speciali
                d[mask] = [file_name]
        else:
            d[mask].append(file_name)

    for k, v in d.items():
        sym_dir_path = os.path.join(os.getcwd(), k)
        os.mkdir(sym_dir_path)
        for file in v:
            link_path = os.path.join(sym_dir_path, file)
            target_path = os.path.join(dir, file)
            os.symlink(target_path, link_path)


if __name__ == "__main__":
    dir = sys.argv[1]
    p = getPermissions(dir)
    createDirs(dir, p)
