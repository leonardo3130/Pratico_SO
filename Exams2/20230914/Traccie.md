### Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere la funzione:
char **vreaddir(const char *path)
che restituisca l'elenco dei nomi dei file in una directory come vettore di stringhe terminato con un puntatore NULL. (lo stesso formato di argv o envp).
Il vettore e le stringhe dei nomi sono allocate dinamicamente.
completare l'esercizio con un programma principale che testi il corretto funzionamento della funzione vreaddir.

### Esercizio 2: Linguaggio C: 10 punti
Rielaborare l'esercizio precedente per fare in modo che il vettore e le stringhe dei nomi siano allocati con una unica operazione malloc (in modo da poter liberare lo spazio usato da vreaddir con una unica operazione free.)


### Esercizio 3: Python o bash: 10 punti
Scrivere un programma python o uno script bash che data una directory passata come parametro produca una lista dei link simbolici presenti nel sottoalbero che fanno riferimento allo stesso file. Esempio, in questo caso:
$ ls -lR /tmp/test
/tmp/test:
total 4
drwxr-xr-x 2 renzo renzo 4096 Sep 10 15:45 d
-rw-r--r-- 1 renzo renzo
-rw-r--r-- 1 renzo renzo
lrwxrwxrwx 1 renzo renzo
lrwxrwxrwx 1 renzo renzo
lrwxrwxrwx 1 renzo renzo
0 Sep 10 15:41 file1
0 Sep 10 15:41 file2
5 Sep 10 15:42 sl1 -> file1
5 Sep 10 15:42 sl1bis -> file1
5 Sep 10 15:49 sl2 -> file2
/tmp/test/d:
total 0
lrwxrwxrwx 1 renzo renzo 15 Sep 10 15:45 gsld -> /tmp/test/file1
lrwxrwxrwx 1 renzo renzo  8 Sep 10 15:43 sld -> ../file1
il programma lanciato con parametro /tmp/test deve trovare che sl1, sl1bis, d/sld e d/gllsd indicano lo stesso file. (similmente dovrebbe rilevare altri insiemi di link simbolici che indicano lo stesso file)