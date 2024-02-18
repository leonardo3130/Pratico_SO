#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FOLDER_NUM 10
//write a program that creates a bunch of olders 

char *itoa(int num, char* str) {
  if(!str)
    return NULL;
  sprintf(str, "%d\n", num);
  return str;
}

int main(){
  pid_t val;
  for(int i = 0; i < FOLDER_NUM; i++) {
    switch (val = fork()) {
      case 0:
        char folderName[512];
        itoa(getpid(), folderName);
        char *commands[] = {"mkdir", folderName, NULL};
        execvp(commands[0], commands);
        break;
      default:
        break; //only new processes creates folders
      case -1:
        perror("fork error");
    }
  }
}

