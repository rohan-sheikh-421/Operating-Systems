#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *source_file = argv[1];
    const char *destination_file = argv[2];

    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        close(pipefd[1]); // Close the write end of the pipe

        // Display child process ID
        printf("[CHILD: %d] Size of Content to Receive: ", getpid());

        // Read the size of the content from the parent
        int content_size;
        read(pipefd[0], &content_size, sizeof(int));
        printf("%d\n", content_size);

        // Read and write the content to the destination file
        int dest_fd = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (dest_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        time_t start_time = time(NULL);
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(dest_fd, buffer, bytes_read);
        }
        time_t end_time = time(NULL);


        // Rewind and display the content received
        lseek(dest_fd, 0, SEEK_SET);
        while ((bytes_read = read(dest_fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }


        // Close the destination file
        close(dest_fd);
        close(pipefd[0]);
    } else { // Parent process
        close(pipefd[0]); // Close the read end of the pipe

        // Display parent process ID
        printf("[PARENT: %d] Size of Content to Send: ", getpid());

        // Open the source file for reading
        int source_fd = open(source_file, O_RDONLY);
        if (source_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Get the size of the source file
        struct stat source_stat;
        if (fstat(source_fd, &source_stat) == -1) {
            perror("fstat");
            exit(EXIT_FAILURE);
        }

        int content_size = source_stat.st_size;
        printf("%d\n", content_size);

        // Send the size of the content to the child
        write(pipefd[1], &content_size, sizeof(int));

        // Read and send the content to the child
        time_t start_time = time(NULL);
        while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
            write(pipefd[1], buffer, bytes_read);
        }
        time_t end_time = time(NULL);

        // Close the write end of the pipe and source file
        close(pipefd[1]);
        close(source_fd);

        // Rewind and display the content sent
        lseek(source_fd, 0, SEEK_SET);
        while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        
    }

    return 0;
}

