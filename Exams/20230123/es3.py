'''
Scrivere uno script bash o python difidr che date due directory ne crei una terza e una quarta
Le nuove directory devono contenere solamente i file aventi lo stesso nome presenti nella prima e nella seconda
albero. Ogni file della terza directory deve contenere una copia del file nella versione della prima directory,
mentre nella quarta directory la versione della seconda.
es.
* se la direcotry a contiene i file alpha, beta, gamma, delta e la directory b i file beta, delta, epsilon, zeta
il comando "difdir a b newa newb" crea le directory newa e newb ed entrambe le directory devono
contenere solo beta e delta (i nomi in comune). newa/beta deve essere una copia di a/beta mentre
newb/beta una copia di b/beta. In modo simile per a/delta b/delta newa/delta e newb/delta.
'''

import os
import sys

def copy_file(src, dst):
    with open(src, 'r') as sfd:
        with open(dst, 'w') as dfd:
            dfd.write(sfd.read())

def difdir(dir1, dir2, new_dir1, new_dir2):
    # Creazione delle nuove directory
    os.makedirs(new_dir1, exist_ok=True)
    os.makedirs(new_dir2, exist_ok=True)

    # Trova i file presenti in entrambe le directory
    common_files = set(os.listdir(dir1)) & set(os.listdir(dir2))

    # Copia i file comuni nelle nuove directory
    for filename in common_files:
        src_file1 = os.path.join(dir1, filename)
        src_file2 = os.path.join(dir2, filename)
        dest_file1 = os.path.join(new_dir1, filename)
        dest_file2 = os.path.join(new_dir2, filename)

        copy_file(src_file1, dest_file1)
        copy_file(src_file2, dest_file2)

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python script.py <dir1> <dir2> <new_dir1> <new_dir2>")
        sys.exit(1)

    dir1 = sys.argv[1]
    dir2 = sys.argv[2]
    new_dir1 = sys.argv[3]
    new_dir2 = sys.argv[4]

    difdir(dir1, dir2, new_dir1, new_dir2)
