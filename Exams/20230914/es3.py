'''
Scrivere un programma python o uno script bash che data una directory passata come parametro
produca una lista dei link simbolici presenti nel sottoalbero che fanno riferimento allo stesso file.
Esempio, in questo caso:
$ ls -lR /tmp/test
/tmp/test:
total 4
drwxr-xr-x 2 renzo renzo 4096 Sep 10 15:45 d
-rw-r--r-- 1 renzo renzo 0 Sep 10 15:41 file1
-rw-r--r-- 1 renzo renzo 0 Sep 10 15:41 file2
lrwxrwxrwx 1 renzo renzo 5 Sep 10 15:42 sl1 -> file1
lrwxrwxrwx 1 renzo renzo 5 Sep 10 15:42 sl1bis -> file1
lrwxrwxrwx 1 renzo renzo 5 Sep 10 15:49 sl2 -> file2
/tmp/test/d:
total 0
lrwxrwxrwx 1 renzo renzo 15 Sep 10 15:45 gsld -> /tmp/test/file1
lrwxrwxrwx 1 renzo renzo 8 Sep 10 15:43 sld -> ../file1
il programma lanciato con parametro /tmp/test deve trovare che sl1, sl1bis, d/sld e d/gllsd indicano lo
stesso file. (similmente dovrebbe rilevare altri insiemi di link simbolici che indicano lo stesso file)
'''

import os

def scan_subtree_symlink(root):
    file_dict = {}
    for dirpath, dirnames, filenames in os.walk(root):
        for filename in filenames:
            file_path = os.path.join(dirpath, filename)
            if os.path.islink(file_path):
                link_target = os.readlink(file_path)        #link del target 
                target_name = os.path.basename(link_target) #nome del target 
                abs_target = os.path.abspath(target_name)   #link assoluto del target
                if abs_target not in file_dict.keys():
                    file_dict[abs_target] = [file_path]
                else:
                    file_dict[abs_target].append(file_path);
    return file_dict
if __name__ == "__main__":
    d = scan_subtree_symlink('.')
    for k,v in d.items():
        print(f'{k} : {v}')
