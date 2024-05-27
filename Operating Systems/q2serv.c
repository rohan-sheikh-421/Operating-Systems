#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>

#define SHARED_MEMORY_SIZE 1024
#define MAX_THREADS 10

sem_t *mutex;
sem_t *readerSemaphore;
sem_t *writerSemaphore;

char *sharedMemory;


void *readSharedMemory(void *arg) {
    // Read the command from the shared memory
    char *command = sharedMemory;
   
    if (strncmp(command, "read", 4) == 0) {
        // read command
    } else if (strncmp(command, "write", 5) == 0) {
        //  write command
    }
    return NULL;
}

int main() {
    //  semaphores
    mutex = sem_open("mutex", O_CREAT, 0644, 1);
    readerSemaphore = sem_open("readerSemaphore", O_CREAT, 0644, 0);
    writerSemaphore = sem_open("writerSemaphore", O_CREAT, 0644, 0);

    // shared memory
    key_t key = ftok("server.c", 'R');
    int shmid = shmget(key, SHARED_MEMORY_SIZE, 0644 | IPC_CREAT);
    sharedMemory = shmat(shmid, (void *)0, 0);

    // threads
    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_create(&threads[i], NULL, readSharedMemory, NULL);
    }

    
    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    
    sem_close(mutex);
    sem_close(readerSemaphore);
    sem_close(writerSemaphore);

    
    shmdt(sharedMemory);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
