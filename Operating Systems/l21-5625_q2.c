#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void displayStudentData(int input_fd) {
    char rollNumber[10];
    int marks;
    char grade;

    while (read(input_fd, rollNumber, sizeof(rollNumber)) > 0 &&
           read(input_fd, &marks, sizeof(int)) > 0 &&
           read(input_fd, &grade, sizeof(char)) > 0) {
        printf("Roll Number: %s\n", rollNumber);
        printf("Marks Gained: %d\n", marks);
        printf("Grade: %c\n", grade);
        printf("_________________\n"); // separator
    }
}

void calculateAndUpdateGrades(int input_fd, int output_fd) {
    char rollNumber[10];
    int marks;
    char grade;

    while (read(input_fd, rollNumber, sizeof(rollNumber)) > 0 &&
           read(input_fd, &marks, sizeof(int)) > 0 &&
           read(input_fd, &grade, sizeof(char)) > 0) {
        printf("Read: %s %d %c\n", rollNumber, marks, grade);

        if (grade == 'I') {
            if (marks >= 80)
                grade = 'A';
            else if (marks >= 70)
                grade = 'B';
            else if (marks >= 60)
                grade = 'C';
            else if (marks >= 50)
                grade = 'D';
            else
                grade = 'F';
        }

        printf("Updated: %s %d %c\n", rollNumber, marks, grade);

        write(output_fd, rollNumber, sizeof(rollNumber));
        write(output_fd, &marks, sizeof(int));
        write(output_fd, &grade, sizeof(char));
    }
}

int main() {
    const char* inputFilename = "file.txt";
    const char* outputFilename = "output2.txt";

    int input_fd = open(inputFilename, O_RDONLY);
    int output_fd = open(outputFilename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (input_fd < 0 || output_fd < 0) {
        perror("File open error");
        return 1;
    }

    pid_t pid1, pid2;
    pid1 = fork();

    if (pid1 < 0) {
        perror("Process not created");
        return 1;
    } else if (pid1 == 0) {
        // Child process 1
        displayStudentData(input_fd);
        close(input_fd);
    } else {
        // Parent process
        pid2 = fork();

        if (pid2 < 0) {
            perror("Process not created");
            return 1;
        } else if (pid2 == 0) {
            // Child process 2
            calculateAndUpdateGrades(input_fd, output_fd);
            close(input_fd);
            close(output_fd);
        } else {
            // Parent process
            wait(NULL); // Wait for the first child
            wait(NULL); // Wait for the second child
            printf("Both child processes completed.\n");
            close(output_fd);
        }
    }

    return 0;
}
