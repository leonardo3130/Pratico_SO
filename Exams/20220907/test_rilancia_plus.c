#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <string1> <string2>\n", argv[0]);
    return 1;
  }

  char *str1 = argv[1];
  char *str2 = argv[2];

  for (int i = 0; i < 5; i++) {
    printf("%s %s %d\n", str1, str2, getpid());
    sleep(1); // sleep for 1 second
  }

  return 0;
}
