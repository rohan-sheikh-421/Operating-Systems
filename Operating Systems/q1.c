#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define maxsize 3

int top = -1;
sem_t push_sem, pop_sem;
int arr[maxsize];
int itemCount = 0;

void *push(void *val)
{
    int value;
    while (true)
    {
        sem_wait(&push_sem);

        value = rand() % 100;
        top = (top + 1) % maxsize;
        arr[top] = value;
        itemCount++;

        printf("Push -> %d\n", value);

        if (itemCount == maxsize)
        {
            printf("STACK FULL\n");
            sem_post(&pop_sem);
            sem_wait(&push_sem);
        }

        sem_post(&push_sem);
        sleep(1); // for visualization
    }
    pthread_exit(NULL);
}

void *pop()
{
    while (true)
    {
        sem_wait(&pop_sem);

        int top_element = arr[top];
        top = (top - 1 + maxsize) % maxsize;
        printf("Pop -> %d\n", top_element);
        itemCount--;

        if (itemCount == 0)
        {
            printf("STACK EMPTY\n");
            sem_post(&push_sem);
            sem_wait(&pop_sem);
        }

        sem_post(&pop_sem);
        sleep(1); // for visualization
    }
    pthread_exit(NULL);
}

int main()
{
    sem_init(&push_sem, 0, 1);
    sem_init(&pop_sem, 0, 0);

    pthread_t push_thread, pop_thread;
    
    pthread_create(&push_thread, NULL, push, NULL);
    pthread_create(&pop_thread, NULL, pop, NULL);

    pthread_join(push_thread, NULL);
    pthread_join(pop_thread, NULL);

    sem_destroy(&push_sem);
    sem_destroy(&pop_sem);

    exit(EXIT_SUCCESS);
}