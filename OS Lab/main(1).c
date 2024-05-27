#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    
    printf("Process ID in main.c: %d\n", getpid());
    char *args[] = {"my_info", NULL};
    execv("./my_info", args);
    // if exec fails
    perror("execv");
    return 1;
}

