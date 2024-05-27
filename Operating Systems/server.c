#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    const char* fifoName = "calculator_fifo";
    mkfifo(fifoName, 0666); // Create a named pipe

    char buffer[256];
    int fd = open(fifoName, O_RDONLY);

    while (1) {
        read(fd, buffer, sizeof(buffer)); // Read data from the client
        char operator;
        double operand1, operand2, result;

        sscanf(buffer, "%c %lf %lf", &operator, &operand1, &operand2);

        switch (operator) {
            case '+':
                result = operand1 + operand2;
                break;
            case '-':
                result = operand1 - operand2;
                break;
            case '*':
                result = operand1 * operand2;
                break;
            case '/':
                if (operand2 != 0) {
                    result = operand1 / operand2;
                } else {
                    printf("Error: Division by zero\n");
                    result = 0;
                }
                break;
            default:
                printf("Error: Invalid operator\n");
                result = 0;
        }

        printf("Server: %c %.2lf %.2lf = %.2lf\n", operator, operand1, operand2, result);
    }

    close(fd);
    unlink(fifoName); // Remove the named pipe when done
    return 0;
}

