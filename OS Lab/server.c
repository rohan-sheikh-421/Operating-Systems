#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    int shmid;
    char *shared_memory;

    // Get the shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    // Attach shared memory segment
    shared_memory = (char *)shmat(shmid, NULL, 0);
    if (shared_memory == (char *)(-1)) {
        perror("shmat");
        return 1;
    }

    // Calculate sum and average
    int sum = 0;
    int count = 0;
    char *token = strtok(shared_memory, " ");
    while (token != NULL) {
        sum += atoi(token);
        count++;
        token = strtok(NULL, " ");
    }

    double average = (double)sum / count;

    printf("Sum = %d\n", sum);
    printf("Average = %.2lf\n", average);

    // Detach shared memory segment
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        return 1;
    }

    // Delete shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        return 1;
    }

    return 0;
}

