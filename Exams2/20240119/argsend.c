#include <stdio.h>

int main(int argc, char **argv){

    for (int i = 0; i < argc; i++) {
        char* arg = argv[i];
        while (*arg != '\0') {
            printf("%c\n", *arg);
            arg++;
        }
    }

    return 0;
}