#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void child_process(int fd_in, int fd_out, off_t offset, off_t length) {
	char buffer[BUFFER_SIZE];

	while (length > 0) {
		ssize_t n = pread(fd_in, buffer, BUFFER_SIZE, offset);	// Legge dal file di input in modo atomico
		pwrite(fd_out, buffer, n, offset);	
		offset += n;
		length -= n;
	}
}

int main(int argc, char *argv[]) {

	int num_processes = 2; // Default
	int opt;

	while ((opt = getopt(argc, argv, "j:")) != -1) {
		switch (opt) {
			case 'j':
				num_processes = atoi(optarg);
				break;
			default:
				fprintf(
						stderr,
						"Uso: %s -j <numero_processi> <file_origine> <file_destinazione>\n",
						argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	if (optind + 2 != argc) {
		fprintf(stderr,
				"Uso: %s -j <numero_processi> <file_origine> <file_destinazione>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	char *file_in = argv[optind];
	char *file_out = argv[optind + 1];

	// Apertura del file in lettura
	int fd_in = open(file_in, O_RDONLY);

	// Apertura del file in scrittura (creazione se non esiste)
	int fd_out = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	off_t file_size = lseek(fd_in, 0, SEEK_END);	// Calcola la dimensione del file

	// Calcola la dimensione della meta  del file
	off_t chunk_size = file_size / num_processes;
	off_t remaining_bytes = file_size % num_processes;
	off_t offset = 0;

	// Creazione dei processi figli
	for (int i = 0; i < num_processes; i++) {
		pid_t pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		} else if (pid == 0) { // Codice eseguito dal processo figlio
			off_t length =
				chunk_size + (i == num_processes - 1 ? remaining_bytes : 0);
			printf("Child %d, offest %d, len %d\n", i, offset, length);
			child_process(fd_in, fd_out, offset, length);
			exit(EXIT_SUCCESS);
		} else { // Codice eseguito dal processo padre
			offset += chunk_size;
		}
	}

	// Codice eseguito dal processo padre
	// Attende la terminazione dei processi figli
	for (int i = 0; i < num_processes; i++) {
		wait(NULL);
	}

	// Chiude i file
	close(fd_in);

	close(fd_out);

	printf("Copia completata con successo.\n");

	return 0;
}
