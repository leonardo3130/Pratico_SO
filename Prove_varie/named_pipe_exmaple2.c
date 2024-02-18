
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int sum(int arr[], int i, int len) {
  if(i == len) 
    return 0;
  return arr[i] + sum(arr, i + 1, len);
}

int main(int argc, char *argv[]) {
  int arr[5];
  srand(time(NULL));

  int fd = open("sum", O_RDONLY);

  read(fd, arr, sizeof(arr));
  printf("Received values\n");
  close(fd);
  printf("Sum: %d", sum(arr, 0, 5));
  return EXIT_SUCCESS;
}

