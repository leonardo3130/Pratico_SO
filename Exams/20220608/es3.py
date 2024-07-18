"""
Scrivere un programma python o uno script bash che esaminando i dati in /proc/*/status fornisca in
output una tabella che indichi per ogni processo di proprietà dell'utente, il nome dell'eseguibile e
l'attuale occupazione di memoria (campo VmSize)
"""

import os
import psutil


def getVmSize(path):
    for line in open(path, "r"):
        if line.startswith("VmSize:"):
            return line.removeprefix("VmSize:")


def createList():
    d = {}
    pids = psutil.pids()
    for pid in pids:
        if os.getuid() in psutil.Process(pid).uids():
            path = f"/proc/{pid}/status"
            d[psutil.Process(pid).name()] = getVmSize(path)
    return d


if __name__ == "__main__":
    d = createList()
    print("|NAME|SIZE|")
    for k, v in d.items():
        print(f"|{k}|{v.strip()}|")
