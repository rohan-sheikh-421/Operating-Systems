#include <stdio.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <string.h>

#define BUFFER_SIZE 20

int main()
{
    // create semaphores and shared memory
    char *buffer;
    int sem1 = shmget(990, sizeof(sem_t), 0666 | IPC_CREAT);
    int sem2 = shmget(991, BUFFER_SIZE, 0666 | IPC_CREAT);
    // semaphore
    sem_t *semaphore;

    // check if semaphores and shared memory are created
    if (sem1 == -1 || sem2 == -1)
    {
        printf("error");
        return 1;
    }

    // check if semaphores and shared memory are attached
    semaphore = (sem_t *)shmat(sem1, NULL, 0);
    buffer = (char *)shmat(sem2, NULL, 0);
    int i = 0;
    while (buffer[0] != '\0')
    {
        sem_wait(semaphore);
        printf("Consumer read: %s", buffer);
        printf("\n");
        sem_post(semaphore);
    }

    // detach and destroy semaphores and shared memory
    shmdt(semaphore);
    shmdt(buffer);

    return 0;
}
