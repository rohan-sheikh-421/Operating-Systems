#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int sum = 0;
    int i;
    for (i = 1; i < argc; i++) {
        sum += atof(argv[i]);
    }
    printf("Sum: %d\n", sum);
    printf("Average: %d\n", sum / (argc - 1));
}
