#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include<string.h>

#define SHARED_MEMORY_SIZE 1024
#define MAX_THREADS 10

sem_t *readerSemaphore;
sem_t *writerSemaphore;

char sharedMemory[SHARED_MEMORY_SIZE];

#define READ_COMMAND "read"
#define WRITE_COMMAND "write"

void processCommand(char *command) {
    if (strcmp(command, READ_COMMAND) == 0) {
        // Process the read command and display the file content
    } else if (strcmp(command, WRITE_COMMAND) == 0) {
        // Process the write command
    }
}

int main() {
    pthread_t thread;
    char command[1024];

    // Initialize semaphores
    readerSemaphore = sem_open("readerSemaphore", O_CREAT, 0);
    writerSemaphore = sem_open("writerSemaphore", O_CREAT, 0);

    // Read command from the terminal
    fgets(command, sizeof(command), stdin);

    // Wait for the shared memory to be ready
    sem_wait(readerSemaphore);

    // Process the command
    processCommand(command);

    // Release the shared memory
    sem_post(readerSemaphore);

    return 0;
}

