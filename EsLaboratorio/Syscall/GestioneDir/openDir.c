#include <stdio.h>
#include <dirent.h>

int main(int argc, char * argv[]) {
	DIR *f = opendir(argv[1]);  // directory da aprire
	struct dirent *d;   // struttura per contenere le informazioni sui file all'interno della directory
	
    /*
        lettura di ogni voce nella directory finché non si raggiunge la fine della directory. 
        Per ogni voce, viene stampato il suo nome utilizzando la funzione printf. 
        La variabile d viene utilizzata per accedere al campo d_name della struttura dirent, 
        che contiene il nome del file.
    */
    while ((d = readdir(f)) != NULL)
		printf("%s\n",d->d_name);
	
	closedir(f);    // chiusura della directory
}