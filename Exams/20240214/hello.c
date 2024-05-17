#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char cwd[1024];
    
    // Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    printf("Hellooooo, %s\n", argv[1]);
    return 0;
}
