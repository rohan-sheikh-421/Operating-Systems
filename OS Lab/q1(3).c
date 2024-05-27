#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void findPalindromesInRow(char* row) {
    char* token = strtok(row, " ");
    while (token != NULL) {
        int isPalindrome = 1;
        int len = strlen(token);
        
        for (int i = 0; i < len / 2; i++) {
            if (token[i] != token[len - 1 - i]) {
                isPalindrome = 0;
                break;
            }
        }
        
        if (isPalindrome) {
            printf("%s ", token);
        }
        
        token = strtok(NULL, " ");
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    FILE* inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        perror("Error opening file for reading");
        return 1;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("Error creating pipe");
        return 1;
    }

    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        return 1;
    }
    
    // Child process 
     
    if (child_pid == 0) 
    {
       
        close(pipe_fd[0]); 

        char line[1024];
        while (fgets(line, sizeof(line), inputFile) != NULL) {
            
            size_t len = strlen(line);
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
            }

            findPalindromesInRow(line);
        }

        
        close(pipe_fd[1]);
        fclose(inputFile);

        return 0;
    } 
    
    // Parent process 
    else 
    {
        
        close(pipe_fd[1]); 

        char buffer[1024];
        ssize_t bytesRead;

        while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead);
        }

        close(pipe_fd[0]);

        wait(NULL);
    }

    return 0;
}

