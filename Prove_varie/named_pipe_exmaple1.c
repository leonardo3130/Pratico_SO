#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[]) {
  int arr[5];
  srand(time(NULL));
  for (int i = 0; i < 5; i++) {
    arr[i] = rand() % 100;
    printf("Generated %d\n", arr[i]);
  }

  mkfifo("sum", 0777);
  int fd = open("sum", O_WRONLY);
  
  write(fd, arr, sizeof(arr));
  close(fd);


  return EXIT_SUCCESS;
}

