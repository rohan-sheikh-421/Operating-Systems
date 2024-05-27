#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ARRAY_SIZE 20

int main() {
    int pipefd_parent_to_child[2];  
    int pipefd_child_to_parent[2];  
    int size, sum = 0;
    int arr[MAX_ARRAY_SIZE];
    int recv_sum;

    // Create pipes
    if (pipe(pipefd_parent_to_child) == -1 || pipe(pipefd_child_to_parent) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Child process
    if (pid == 0) { 
        close(pipefd_parent_to_child[1]);  
        close(pipefd_child_to_parent[0]);  

        // Receive array from parent
        int num_read = read(pipefd_parent_to_child[0], arr, sizeof(arr));
        if (num_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Calculate sum
        size = num_read / sizeof(int);
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }

        // Send sum to parent
        write(pipefd_child_to_parent[1], &sum, sizeof(int));
        
        close(pipefd_parent_to_child[0]);  
        close(pipefd_child_to_parent[1]);  
    } 
    // Parent process
    else { 
        close(pipefd_parent_to_child[0]);  
        close(pipefd_child_to_parent[1]);  

        int size = 5;
        int parent_arr[] = {15, 87, 61, 9, 8};

        // Send array to child
        write(pipefd_parent_to_child[1], parent_arr, sizeof(int) * size);

        // Read sum from child
        read(pipefd_child_to_parent[0], &recv_sum, sizeof(int));

        printf("Parent Process: Sum received from child = %d\n", recv_sum);

        close(pipefd_parent_to_child[1]);  
        close(pipefd_child_to_parent[0]);  
    }

    return 0;
}

