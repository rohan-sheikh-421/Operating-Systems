#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    const char* fifoName = "calculator_fifo";
    int fd = open(fifoName, O_WRONLY);

    if (fd == -1) {
        perror("Error opening the named pipe (server is not running)");
        exit(1);
    }

    char input[256];
    char result[256];

    while (1) {
        printf("Enter operator and two operands (e.g., + 4 10): ");
        fgets(input, sizeof(input), stdin);

        // Write the data to the server
        write(fd, input, strlen(input) + 1);

        // Read and display the result from the server
        read(fd, result, sizeof(result));
        printf("Client: %s", result);
    }

    close(fd);
    return 0;
}

