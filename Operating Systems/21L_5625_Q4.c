#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3

// Structure to hold thread-specific data
struct ThreadData {
    int* numbers;
    int size;
};

// Function to calculate the average
void* calculateAverage(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;
    int* numbers = threadData->numbers;
    int size = threadData->size;

    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += numbers[i];
    }

    double average = sum / size;
    pthread_exit((void*)&average);
}

// Function to calculate the maximum
void* calculateMaximum(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;
    int* numbers = threadData->numbers;
    int size = threadData->size;

    int max = numbers[0];
    for (int i = 1; i < size; i++) {
        if (numbers[i] > max) {
            max = numbers[i];
        }
    }

    pthread_exit((void*)&max);
}

// Function to calculate the minimum
void* calculateMinimum(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;
    int* numbers = threadData->numbers;
    int size = threadData->size;

    int min = numbers[0];
    for (int i = 1; i < size; i++) {
        if (numbers[i] < min) {
            min = numbers[i];
        }
    }

    pthread_exit((void*)&min);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number1> <number2> ... <numberN>\n", argv[0]);
        return 1;
    }

    int size = argc - 1;
    int numbers[size];

    for (int i = 1; i < argc; i++) {
        numbers[i - 1] = atoi(argv[i]);
    }

    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];

    pthread_create(&threads[0], NULL, calculateAverage, &threadData[0]);
    pthread_create(&threads[1], NULL, calculateMaximum, &threadData[1]);
    pthread_create(&threads[2], NULL, calculateMinimum, &threadData[2]);

    // Set thread-specific data
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].numbers = numbers;
        threadData[i].size = size;
    }

    double average;
    int maximum, minimum;

    pthread_join(threads[0], (void**)&average);
    pthread_join(threads[1], (void**)&maximum);
    pthread_join(threads[2], (void**)&minimum);

    printf("Average: %.2f\n", average);
    printf("Maximum: %d\n", maximum);
    printf("Minimum: %d\n", minimum);

    return 0;
}

