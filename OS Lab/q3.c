#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int N;
int* fibonacciSeries;
int evenCount = 0;
int oddCount = 0;
int sum = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_even = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_odd = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_sum = PTHREAD_COND_INITIALIZER;

void* fibonacciGenerator(void* arg) {
    int n = *((int*)arg);
    int a = 0, b = 1;

    pthread_mutex_lock(&mutex);

    printf("ID = %lu, Series: 0 ", pthread_self());
    fibonacciSeries[0] = 0;

    if (n > 0) {
        printf("1 ");
        fibonacciSeries[1] = 1;
    }

    for (int i = 2; i < n; i++) {
        int next = a + b;
        a = b;
        b = next;
        fibonacciSeries[i] = next;
        printf("%d ", next);
    }
    printf("\n");

    pthread_cond_signal(&cond_even);
    pthread_cond_wait(&cond_odd, &mutex);

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* evenCounter(void* arg) {
    pthread_mutex_lock(&mutex);
    
    pthread_cond_wait(&cond_even, &mutex); 
    printf("ID = %lu, Even Numbers: ", pthread_self());
    
    for (int i = 0; i < N; i++) {
        if (fibonacciSeries[i] % 2 == 0) {
            evenCount++;
        }
    }

    printf("%d\n", evenCount);
    
    pthread_cond_signal(&cond_odd); 
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* oddCounter(void* arg) {
    pthread_mutex_lock(&mutex);

    pthread_cond_wait(&cond_odd, &mutex); 
    printf("ID = %lu, Odd Numbers: ", pthread_self());

    for (int i = 0; i < N; i++) {
        if (fibonacciSeries[i] % 2 != 0) {
            oddCount++;
        }
    }

    printf("%d\n", oddCount); 
    
    pthread_cond_signal(&cond_sum); 
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


void* sumCalculator(void* arg) {
    pthread_mutex_lock(&mutex);

    pthread_cond_wait(&cond_sum, &mutex);
    printf("ID = %lu, Sum: ", pthread_self());

    for (int i = 0; i < N; i++) {
        sum += fibonacciSeries[i];
    }

    printf("%d\n", sum);

    FILE* sumFile = fopen("sum.txt", "w");
    fprintf(sumFile, "%d", sum);
    fclose(sumFile);

    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "N must be a positive integer.\n");
        return 1;
    }

    fibonacciSeries = (int*)malloc(N * sizeof(int));

    pthread_t generatorThread, evenThread, oddThread, sumThread;

    pthread_create(&generatorThread, NULL, fibonacciGenerator, &N);
    pthread_create(&evenThread, NULL, evenCounter, NULL);
    pthread_create(&oddThread, NULL, oddCounter, NULL);
    pthread_create(&sumThread, NULL, sumCalculator, NULL);

    pthread_join(generatorThread, NULL);
    pthread_join(evenThread, NULL);
    pthread_join(oddThread, NULL);
    pthread_join(sumThread, NULL);

    free(fibonacciSeries);

    return 0;
}

