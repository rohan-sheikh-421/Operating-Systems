#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 11) {
        printf("Usage: %s <num1> <num2> <num3> <num4> <num5> <num6> <num7> <num8> <num9> <num10>\n", argv[0]);
        return 1;
    }

    int arr[10];
    for (int i = 1; i < 10; i++) {
        arr[i] = atoi(argv[i + 1]);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        bubbleSort(arr, 10);
        printf("Sorted array by child process: ");
        for (int i = 1; i < 10; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        printf("Child process ID: %d\n", getpid());
    } else {
        // Parent process
        wait(NULL); // Wait for the child to complete
    }

    return 0;
}


