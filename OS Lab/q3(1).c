#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

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
    
    //child process

    if (pid == 0) { 
            close(pipefd[1]); // Close the write end of the pipe

        // Open the destination file for writing
        int dest_fd = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (dest_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Read the size of the content from the parent
        int content_size;
        read(pipefd[0], &content_size, sizeof(int));

        // Read and write the content to the destination file
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(dest_fd, buffer, bytes_read);
        }

        close(pipefd[0]); 
        close(dest_fd);   
    } 
    
    // Parent process
    
    else 
    { 
        close(pipefd[0]); 

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

        // Send the size of the content to the child
        write(pipefd[1], &content_size, sizeof(int));

        // Read and send the content to the child
        while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
            write(pipefd[1], buffer, bytes_read);
        }

        close(pipefd[1]); 
        close(source_fd); 
    }

    return 0;
}

