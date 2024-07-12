"""
Scrivere uno script bash o python che faccia il merge di due alberi del file system copiandoli in un terzo.
La gerarchia risultante dovrebbe contenere tutti i file e le directory presenti nel primo o nel secondo albero.
Se due file hanno lo stesso percorso e nomi uguali nei due alberi di partenza i contenuti devono essere
concatenati nel file risultante.
"""

import os
import sys


def copy_or_concatenate(f1, f2):
    with open(f1, "r") as firstfile, open(f2, "a") as secondfile:
        for line in firstfile:
            secondfile.write(line)


def mergeDirs(dir1, dir2):

    for root, dirs, files in os.walk(dir1):
        root_list3 = root.split("/")
        root_list3[0] = dir3  # dir3 al posto di dir1
        root3 = "/".join(root_list3)
        for file in files:
            copy_or_concatenate(os.path.join(root, file), os.path.join(root3, file))
        for dir in dirs:
            if not os.path.exists(os.path.join(root3, dir)):
                # potrebbe essere già stata create
                # con un file
                os.mkdir(os.path.join(root3, dir))

    for root, dirs, files in os.walk(dir2):
        root_list1 = root.split("/")
        root_list1[0] = dir1
        root1 = "/".join(root_list1)
        root_list3 = root.split("/")
        root_list3[0] = dir3  # dir3 al posto di dir2
        root3 = "/".join(root_list3)
        for file in files:
            copy_or_concatenate(os.path.join(root, file), os.path.join(root3, file))

        for dir in dirs:
            if not os.path.exists(os.path.join(root3, dir)):
                os.mkdir(os.path.join(root3, dir))


if __name__ == "__main__":
    dir1 = sys.argv[1]
    dir2 = sys.argv[2]
    dir3 = sys.argv[3]
    mergeDirs(dir1, dir2)
