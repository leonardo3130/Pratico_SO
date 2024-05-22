'''
Scrivere uno script bash o python che faccia il merge di due alberi del file system copiandoli in un terzo.
La gerarchia risultante dovrebbe contenere tutti i file e le directory presenti nel primo o nel secondo albero.
Se due file hanno lo stesso percorso e nomi uguali nei due alberi di partenza i contenuti devono essere
concatenati nel file risultante.
'''
import os

def concatenate_files(src1, src2, dst):
    with open(dst, 'w') as wfd:
        with open(src1, 'r') as fd:
            wfd.write(fd.read())
        with open(src2, 'r') as fd:
            wfd.write(fd.read())

def copy_file(src, dst):
    with open(src, 'r') as sfd:
        with open(dst, 'w') as dfd:
            dfd.write(sfd.read())

def merge_trees(src_tree1, src_tree2, dst_tree):
    for root, dirs, files in os.walk(src_tree1):
        rel_path = os.path.relpath(root, src_tree1)
        dst_path = os.path.join(dst_tree, rel_path)
        os.makedirs(dst_path, exist_ok=True)

        for file in files:
            src_file1 = os.path.join(root, file)
            src_file2 = os.path.join(src_tree2, rel_path, file)
            dst_file = os.path.join(dst_path, file)

            if os.path.exists(src_file2):
                concatenate_files(src_file1, src_file2, dst_file)
            else:
                copy_file(src_file1, dst_file)

    for root, dirs, files in os.walk(src_tree2):
        rel_path = os.path.relpath(root, src_tree2)
        dst_path = os.path.join(dst_tree, rel_path)
        os.makedirs(dst_path, exist_ok=True)

        for file in files:
            src_file2 = os.path.join(root, file)
            dst_file = os.path.join(dst_path, file)

            if not os.path.exists(dst_file):
                copy_file(src_file2, dst_file)

if __name__ == "__main__":
    src_tree1 = '/path/to/src_tree1'
    src_tree2 = '/path/to/src_tree2'
    dst_tree = '/path/to/dst_tree'

    merge_trees(src_tree1, src_tree2, dst_tree)
