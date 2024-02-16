#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t val;
  switch (val = fork()) {
    case 0:
      printf("Sono il figlio, id: %d, id di mio padre: %d\n", getpid(), getppid());
      sleep(5);
      _exit(42);
      break;
    default:
      printf("Sono il padre, id: %d, id di mio figlio: %d\n", getpid(), val);
      //attesa del figlio
      int status;
      waitpid(val, &status, NULL);
      printf("Il processo figlio ha terminato l'esecuzione con exit value: %d\n", WEXITSTATUS(status));
      break;
    case -1:
      write(2, "Error", 10);
  }
}
