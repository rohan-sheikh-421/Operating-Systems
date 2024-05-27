#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

#define SHARED_MEMORY_SIZE 1024

sem_t *readerSemaphore;
sem_t *writerSemaphore;

char *sharedMemory;

int main() {
    // semaphores
    readerSemaphore = sem_open("readerSemaphore", O_CREAT, 0644, 0);
    writerSemaphore = sem_open("writerSemaphore", O_CREAT, 0644, 0);

    // shared memory
    key_t key = ftok("server.c", 'R');
    int shmid = shmget(key, SHARED_MEMORY_SIZE, 0644);
    sharedMemory = shmat(shmid, (void *)0, 0);

    // Read command 
    char command[1024];
    fgets(command, sizeof(command), stdin);

    // Write command 
    strncpy(sharedMemory, command, SHARED_MEMORY_SIZE);

    
    sem_post(readerSemaphore);

    
    if (strncmp(command, "read", 4) == 0) {
        sem_wait(writerSemaphore);
      
    }

    
    shmdt(sharedMemory);

    return 0;
}
