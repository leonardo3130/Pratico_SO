#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 1024

int main(int argc, char *argv[]) {
  int pid = getpid();
  for (;;) {
    printf("I am %d, %s\n", pid, argv[1]);
    sleep(2);
  }
  return EXIT_SUCCESS;
}
