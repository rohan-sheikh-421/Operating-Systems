#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *pid_disp(void *arg)
{
    printf("Thread ID: %ld\n", pthread_self());
    printf("Process ID: %d\n", getpid());
    pthread_exit(NULL);
}

int main() {
    int size = 7;
    pthread_t threads[size];

    for (int i = 0; i < size; i++) {
        pthread_create(&threads[i], NULL,pid_disp, NULL);
    }

    for (int i = 0; i < size; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

