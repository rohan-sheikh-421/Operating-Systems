#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N;
int sum = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* summationFunction(void* arg) {
    int n = *((int*)arg);

    for (int i = 0; i <= n; i++) {
        pthread_mutex_lock(&mutex);
        sum += i;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    if (N < 0) {
        fprintf(stderr, "N must be a non-negative integer.\n");
        return 1;
    }

    pthread_t summationThread;

    pthread_create(&summationThread, NULL, summationFunction, &N);

    pthread_join(summationThread, NULL);

    printf("Sum of integers from 0 to %d is: %d\n", N, sum);

    return 0;
}

