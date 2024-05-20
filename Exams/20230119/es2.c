/*
Estendere il programma stdin2pipe in modo da consentire più di due righe di input.
Per esempio, dato il file cmds che contiene 4 righe
ls -l
awk '{print $1}'
sort
uniq
l'esecuzione di:
stdin2pipe < cmds
sia equivalente al comando:
ls -l | awk '{print $1}' | sort | uniq'
*/
