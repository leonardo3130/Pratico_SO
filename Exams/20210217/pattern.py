"""
Scrivere uno script bash o un programma python che preso come parametro un pattern (stringa
ASCII) fornisca in output l'elenco dei file del sottoalbero che ha come radice la directory corrente che
nel loro contenuto includano il pattern.. La lista di output deve essere ordinata dal file più
recentemente modificato al file con ultima modifica più remota.
"""

import sys
import os


def is_executable(pathname):
    if os.path.isfile(pathname):
        return os.access(pathname, os.X_OK)
    return False


def contains(file_path, pattern):
    if is_executable(file_path):
        return False
    with open(file_path, "r") as f:
        for line in f:
            if pattern in line:
                return True
    return False


def find_pattern(root, pattern):
    l = []
    for root, dirs, files in os.walk(root):
        for file in files:
            file_path = os.path.join(root, file)
            if contains(file_path, pattern):
                l.append(file_path)
    l.sort(key=lambda x: os.path.getmtime(x), reverse=True)
    return l


if __name__ == "__main__":
    root = os.getcwd()
    pattern = sys.argv[1]
    l = find_pattern(root, pattern)
    for el in l:
        print(el)
    pass
