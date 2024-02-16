#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//write a program that calculates factorial using 2way communication wit pipes

int main(int argc, char *argv[]) {
  int c2p[2];
  int p2c[2];
  int val;
  int n = 5;
  int first = 1;
  
  pipe(c2p);
  pipe(p2c);

  int done = 0;
  val = fork();

  if(val == -1) {
    perror("fork error");
  }
  else {
    while(!done) {
      switch (val) {
        case 0:
          close(c2p[0]);
          close(p2c[1]);
          int bufferc[2];
          read(p2c[0], &bufferc[0], sizeof(bufferc));
          if(!bufferc[0]) {
            done = 1;
            printf("factorial = %d", bufferc[1]);
          }
          else if(bufferc[0] == 1) {
            bufferc[1] *= bufferc[0];
            bufferc[0]--;
            printf("factorial = %d", bufferc[1]);
            write(c2p[1], &bufferc, sizeof(bufferc));
            done = 1;
          }
          else {
            bufferc[1] *= bufferc[0];
            bufferc[0]--;
            write(c2p[1], &bufferc, sizeof(bufferc));
          }
          close(c2p[1]);
          close(p2c[0]);
          break;
        default:
          close(c2p[1]);
          close(p2c[0]);
          int bufferp[2];
          if(first) {
            bufferp[0] = n; bufferp[1] = 1; first = 0;
            write(p2c[1], &bufferp, sizeof(bufferp));
          }
          else {
            read(c2p[0], &bufferp[0], sizeof(bufferp));
            if(!bufferp[0]) {
              done = 1;
              printf("factorial = %d", bufferp[1]);
            }
            else if(bufferp[0] == 1) {
              bufferp[1] *= bufferp[0];
              bufferp[0]--;
              printf("factorial = %d", bufferp[1]);
              write(p2c[1], &bufferp, sizeof(bufferp));
              done = 1;
            }
            else {
              bufferp[1] *= bufferp[0];
              bufferp[0]--;
              write(c2p[1], &bufferp, sizeof(bufferp));
            }
          }
          close(c2p[0]);
          close(p2c[1]);
          break;
      }
    }
  }
  return EXIT_SUCCESS;
}
