#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open file\n");
        return 1;
    }

    int pid1 = fork();
    if (pid1 == -1) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid1 == 0) {
        // This is the first child process
        char buffer[1024];
        int bytesRead;
        while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        close(fd);
    } else {
        int pid2 = fork();
        if (pid2 == -1) {
            fprintf(stderr, "Fork failed\n");
            return 1;
        } else if (pid2 == 0) {
            // This is the second child process
            int outputFd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            char rollNumber[256];
            int marks;
            char grade;
            while (scanf("%s %d %c", rollNumber, &marks, &grade) == 3) {
                if (grade == 'I') {
                    if (marks >= 80 && marks <= 100) {
                        grade = 'A';
                    } else if (marks >= 70 && marks <= 79) {
                        grade = 'B';
                    } else if (marks >= 60 && marks <= 69) {
                        grade = 'C';
                    } else if (marks >= 50 && marks <= 59) {
                        grade = 'D';
                    } else {
                        grade = 'F';
                    }
                }
                dprintf(outputFd, "%s %d %c\n", rollNumber, marks, grade);
            }
            close(outputFd);
        } else {
            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}
