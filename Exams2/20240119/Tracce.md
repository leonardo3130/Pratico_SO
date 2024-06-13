Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere un programma argsend che converta i parametri del programma (da argv[1] in poi) in una unica sequenza di caratteri: vengono concatenati i parametri (compreso il terminatore della stringa).

Esempio di funzionamento:
$ ./argsend ls -l /tmp | od -c
0000000 l s \0 - l \0 / t m p \0
0000013