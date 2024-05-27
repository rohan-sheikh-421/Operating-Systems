#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<ctype.h>

#define BUFFER_SIZE 256

int main() {
    int pipefd[2];
    pid_t pid;
    char message[BUFFER_SIZE];
    char changedMessage[BUFFER_SIZE];

    // pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
     // Child process
    if (pid == 0) { 
        close(pipefd[1]);

        ssize_t bytesRead = read(pipefd[0], message, sizeof(message));
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // changing case of message
        for (int i = 0; i < bytesRead; i++) {
            if (islower(message[i])) {
                changedMessage[i] = toupper(message[i]);
            } else if (isupper(message[i])) {
                changedMessage[i] = tolower(message[i]);
            } else {
                changedMessage[i] = message[i];
            }
        }

        printf("Child Process: Original message - %s\n", message);
        printf("Child Process: changed case message - %s\n", changedMessage);

        close(pipefd[0]); 
    } 
    
    // Parent process
    else { 
        close(pipefd[0]); 

        strcpy(message, "Hi There");

        printf("Parent Process: Sending message - %s\n", message);

        ssize_t bytesWritten = write(pipefd[1], message, strlen(message) + 1);
        if (bytesWritten == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]); 
    }
    
    return 0;
}

