### Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere un programma search_name che deve cercare nel sottoalbero della directory corrente tutti i
file eseguibili con un nome file specifico passato come primo e unico parametro indicando per ogni
file il tipo di eseguibile (script o eseguibile binario).

Ad esempio il comando:
./search_name testprog
    deve cercare i file eseguibili chiamati testprog nell'albero della directory corrente. Poniamo
siano ./testprog, ./dir1/testprog, ./dir/dir3/testprog, search_name deve stampare:
. /testprog: script
./dir1/testprog: ELF executable
./dir/dir3/testprog: ELF executable

Il programma utilizza diverse syscall per operazioni di file system e input/output. Ecco una discussione delle syscall utilizzate con una breve descrizione e alcuni esempi d'uso:

1. **`opendir`**:
   - **Descrizione**: Apre una directory per la lettura delle sue entry.
   - **Esempio d'uso**: Quando si vuole iniziare a leggere il contenuto di una directory. Ad esempio, si apre la directory corrente (`"."`) per esplorarne il contenuto.

2. **`readdir`**:
   - **Descrizione**: Legge una entry dalla directory aperta con `opendir`.
   - **Esempio d'uso**: Usato in un ciclo per leggere tutte le entry di una directory, restituendo i nomi dei file e delle sottodirectory in essa contenuti.

3. **`closedir`**:
   - **Descrizione**: Chiude una directory precedentemente aperta con `opendir`.
   - **Esempio d'uso**: Dopo aver finito di leggere una directory con `readdir`, si chiude la directory per liberare risorse.

4. **`stat`**:
   - **Descrizione**: Recupera informazioni sul file specificato (ad esempio, se è un file regolare, una directory, i permessi, ecc.).
   - **Esempio d'uso**: Determina il tipo di un file e i suoi permessi, utile per sapere se una entry di directory è un file o una sottodirectory.

5. **`access`**:
   - **Descrizione**: Verifica i permessi di accesso per un file. Può controllare se un file esiste, è leggibile, scrivibile o eseguibile.
    Values for the second argument to access.
    These may be OR'd together.  
    #define	R_OK	4		/* Test for read permission.  */
    #define	W_OK	2		/* Test for write permission.  */
    #define	X_OK	1		/* Test for execute permission.  */
    #define	F_OK	0		/* Test for existence.  */

   - **Esempio d'uso**: Controlla se un file specifico è eseguibile prima di tentare di determinarne il tipo di eseguibile.

6. **`fopen`**:
   - **Descrizione**: Apre un file per la lettura o scrittura.
   - **Esempio d'uso**: Apre un file in modalità lettura per controllarne i primi byte e determinare se è uno script o un eseguibile ELF.

7. **`fread`**:
   - **Descrizione**: Legge dati da un file aperto con `fopen`.
   - **Esempio d'uso**: Legge i primi byte di un file per determinare la sua firma magica (ad esempio, `#!` per script o `\x7fELF` per eseguibili ELF).

8. **`fclose`**:
   - **Descrizione**: Chiude un file precedentemente aperto con `fopen`.
   - **Esempio d'uso**: Chiude un file dopo averne letto i dati per determinare il tipo di eseguibile, liberando così le risorse associate.

### Esempi d'Uso delle Syscall (Descrittivi):

- **`opendir` e `readdir`**:
  Immagina di voler esplorare una directory per trovare tutti i file presenti. Apri la directory con `opendir` e poi usi `readdir` in un ciclo per ottenere ogni file e directory contenuti in essa.

- **`stat`**:
  Hai bisogno di sapere se una specifica entry è una directory o un file regolare. Usando `stat`, ottieni queste informazioni e puoi quindi decidere come procedere (ad esempio, se è una directory, esplorarla ricorsivamente).

- **`access`**:
  Prima di eseguire un file, vuoi assicurarti che sia davvero eseguibile dall'utente corrente. Usi `access` con il flag `X_OK` per verificare i permessi di esecuzione.

- **`fopen`, `fread`, e `fclose`**:
  Per determinare se un file è uno script, apri il file con `fopen`, leggi i primi due byte con `fread` e poi chiudi il file con `fclose`. Se i primi due byte sono `#!`, sai che è uno script.

Queste syscall sono fondamentali per interagire con il file system a basso livello e permettono di costruire applicazioni che possono esplorare directory, leggere file e determinare attributi dei file stessi.

<hr>

### Esercizio 2: Linguaggio C: 10 punti
Il programma run_name deve cercare nel sottoalbero della directory corrente tutti i file eseguibili con
un nome file specifico (primo parametro di run_name) e li deve mettere in esecuzione uno dopo l'altro
passando i successivi parametri.
Ad esempio il comando:
./run_name testprog a b c
deve cercare i file eseguibili chiamati testprog nell'albero della directory corrente. Poniamo
siano ./testprog, ./dir1/testprog, ./dir/dir3/testprog, run_name deve eseguire
testprog a b c
per 3 volte. Nella prima esecuzione la working directory deve essere la dir corrente '.', la seconda
deve avere come working directory './dir1' e la terza './dir2/dir3'.