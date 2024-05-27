#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n, int ascending) {
    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < n - i - 1; j++) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
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

    pid_t child1, child2;

    child1 = fork();
    if (child1 == 0) {
        // Child 1
        printf("Child 1 ID: %d, Parent ID: %d\n", getpid(), getppid());
        bubbleSort(arr, 10, 1);
        printf("Child 1 sorted array in ascending order: ");
        for (int i = 1; i < 10; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        exit(0);
    } else {
        child2 = fork();
        if (child2 == 0) {
            // Child 2
            printf("Child 2 ID: %d, Parent ID: %d\n", getpid(), getppid());
            bubbleSort(arr, 10, 0);
            printf("Child 2 sorted array in descending order: ");
            for (int i = 1; i < 10; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
            exit(0);
        } else {
            // Parent process
            waitpid(child1, NULL, 0);
            waitpid(child2, NULL, 0);
            printf("Parent ID: %d, Parent's Parent ID: %d\n", getpid(), getppid());
        }
    }

    return 0;
}


