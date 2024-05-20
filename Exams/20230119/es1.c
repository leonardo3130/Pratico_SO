/*
Scrivere il programma stdin2pipe che prenda in input due righe di testo, ogni riga contiene un
comando coi rispettivi parametri. Il programma deve lanciare entrambi i comandi in modo tale
l'output del primo diventi input del secondo.
Per esempio, dato il file cmds:
ls -l
awk '{print $1}'
l'esecuzione di:
stdin2pipe < cmds
sia equivalente al comando:
ls -l | awk '{print $1}'
*/
