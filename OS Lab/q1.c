#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


sem_t *sem_a;
sem_t *sem_b;
sem_t *sem_c;

void *thread_function(void *arg)
{
    int a = (int)arg;
    sem_t *semaphores[3] = {sem_a, sem_b, sem_c};

    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            sem_wait(semaphores[i]);
        }

        switch (a)
        {
            case 0:
                printf("aaabaaabaaabcaaabaaab");
                break;
            case 1:
                printf("abaabacabaac");
                break;
            case 2:
                printf("abcaabcabca");
                break;
        }

        for (int i = 0; i < 3; i++)
        {
            sem_post(semaphores[i]);
        }
    }

    return NULL;
}

int main()
{
    pthread_t thread1, thread2, thread3;
    //roll number is 5625
    int roll_number = 25;
    int last_digit = roll_number % 10;
    int a = last_digit % 3;

  

    if (pthread_create(&thread1, NULL, thread_function, (void *)&a) != 0 ||
        pthread_create(&thread2, NULL, thread_function, (void *)&a) != 0 ||
        pthread_create(&thread3, NULL, thread_function, (void *)&a) != 0)
    {
        perror("Thread creation failed");
        return 1;
    }

   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);
   pthread_join(thread3, NULL);

    sem_close(sem_a);
    sem_close(sem_b);
    sem_close(sem_c);

    return 0;
}






