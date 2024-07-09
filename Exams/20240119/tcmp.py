"""
Scrivere un programma python o uno script bash chiamato tcmp che confronti gli alberi del file
system di due directory.. A seconda del parametro deve elencare i pathname di file e di directory che
• sono comuni ad entrambi i sottoalberi, se manca il parametro
• esistono solo nel primo sottoalbero, se il parametro è -1
• esistono solo nel secondo sottoalbero se il parametro è -2
esempi:
$ . /tcmp dir1 dir2
stampa l'elenco dei path che esistono sia in dir1 sia in dir2
$ . /tmcp -1 dir1 dir2
stampa l'elenco dei path che esistono in dir1 ma non in dir2
"""

import subprocess as sb
import os
import sys


def tcmp(dir1, dir2, flag):
    paths1 = scan_subtree(dir1)
    paths2 = scan_subtree(dir2)
    paths = set()
    if flag:
        paths = paths1 - paths2  # set difference
    else:
        paths = paths1 & paths2  # set intersection

    for path in paths:
        print(path)


def scan_subtree(root):
    paths = set()
    for dirpath, dirnames, filenames in os.walk(root):
        for filename in filenames:
            path = os.path.join(dirpath, filename)
            path_list = path.split("/")
            path_list = path_list[2:]
            path = "/".join(path_list)
            paths.add(path)
    return paths


if __name__ == "__main__":
    dir1 = dir2 = ""
    flag = False
    if sys.argv[1] == "-1":
        dir1 = sys.argv[2]
        dir2 = sys.argv[3]
        flag = True
    elif sys.argv[1] == "-2":
        dir1 = sys.argv[3]
        dir2 = sys.argv[2]
        flag = True
    else:
        dir1 = sys.argv[1]
        dir2 = sys.argv[2]
    tcmp(dir1, dir2, flag)
