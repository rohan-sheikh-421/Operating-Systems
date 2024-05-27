#include <stdio.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <string.h>

#define BUFFER_SIZE 20

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <source_file_name>\n", argv[0]);
        return 1;
    }

    // Open the source file
    FILE *source_file = fopen(argv[1], "rb");
    if (!source_file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    // create semaphores and shared memory
    char *buffer;
    int sem1 = shmget(990, sizeof(sem_t), 0666 | IPC_CREAT);
    int sem2 = shmget(991, BUFFER_SIZE, 0666 | IPC_CREAT);

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
    // initialize semaphore
    sem_init(semaphore, 1, 0);

    // write to shared memory
    while (fgets(buffer, BUFFER_SIZE, source_file) != NULL)
    {
        printf("Producer wrote: %s", buffer);
        printf("\n");
        sem_post(semaphore);
        sem_wait(semaphore);
    }
    printf(" sent to consumer ");
    buffer[0] = '$';
    sem_post(semaphore);

    // detach and destroy semaphores and shared memory
    sem_destroy(semaphore);
    shmdt(semaphore);
    shmdt(buffer);
    fclose(source_file);

    return 0;
}
