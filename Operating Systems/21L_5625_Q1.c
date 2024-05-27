#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>


int sumFactorsInRange(int start, int end, int num) {
    int sum = 0;
    for (int i = start; i <= end; i++) {
        if (num % i == 0) {
            sum += i;
        }
    }
    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int status;
    bool isPerfect = false;

    int fd1[2]; // Pipe 1
    int fd2[2]; // Pipe 2

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t pid1, pid2;

    // First child process
    pid1 = fork();
    if (pid1 == 0) {
        // Child 1
        close(fd1[0]);
        int sum1 = sumFactorsInRange(1, N / 4, N);
        write(fd1[1], &sum1, sizeof(sum1));
        close(fd1[1]);
        exit(0);
    } else if (pid1 < 0) {
        perror("Fork 1 failed");
        return 1;
    }

    // Child 2
    pid2 = fork();
    if (pid2 == 0) {
        
        close(fd2[0]);
        int sum2 = sumFactorsInRange(N / 4 + 1, N / 2, N);
        write(fd2[1], &sum2, sizeof(sum2));
        close(fd2[1]);
        exit(0);
    } else if (pid2 < 0) {
        perror("Fork 2 failed");
        return 1;
    }

    // Parent process
    close(fd1[1]);
    close(fd2[1]);

    int sum1, sum2;
    read(fd1[0], &sum1, sizeof(sum1));
    read(fd2[0], &sum2, sizeof(sum2));

    close(fd1[0]);
    close(fd2[0]);

    
    int sum3 = sumFactorsInRange(N / 2 + 1, N - 1, N);

    int totalSum = sum1 + sum2 + sum3;

    if (totalSum == N) {
        isPerfect = true;
    }

    printf("%d is%s a perfect number.\n", N, isPerfect ? "" : " not");

    
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    return 0;
}

