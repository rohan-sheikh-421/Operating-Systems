#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_WORD_LEN 100

// Structure to store word frequency data
typedef struct {
    char word[MAX_WORD_LEN];
    int frequency;
} WordData;

// Structure to pass data to each thread
typedef struct {
    char* segment;
    WordData* wordData;
    int numWords;
} ThreadData;

// Function to parse words from a segment and update word frequency
void* processSegment(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    char* segment = data->segment;
    WordData* wordData = data->wordData;
    int numWords = data->numWords;

    char* word = strtok(segment, " \t\n");
    while (word != NULL) {
        // Search for the word in wordData
        int foundIndex = -1;
        for (int i = 0; i < numWords; i++) {
            if (strcmp(word, wordData[i].word) == 0) {
                foundIndex = i;
                break;
            }
        }

        // If the word is found, increment its frequency
        if (foundIndex != -1) {
            wordData[foundIndex].frequency++;
        } else {
            // If the word is not found, add it to wordData
            int newIndex = numWords;
            strcpy(wordData[newIndex].word, word);
            wordData[newIndex].frequency = 1;
            numWords++;
        }

        word = strtok(NULL, " \t\n");
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <num_segments>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    int numSegments = atoi(argv[2]);

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("File open error");
        return 1;
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    int segmentSize = fileSize / numSegments;
    WordData wordData[numSegments][fileSize / 2]; // Assuming an average word length of 2

    char* fileData = (char*)malloc(fileSize);
    if (read(fd, fileData, fileSize) == -1) {
        perror("File read error");
        return 1;
    }
    close(fd);

    pthread_t threads[numSegments];
    ThreadData threadData[numSegments];

    int startPos = 0;
    for (int i = 0; i < numSegments; i++) {
        threadData[i].segment = &fileData[startPos];
        threadData[i].wordData = wordData[i];
        threadData[i].numWords = 0;

        startPos += segmentSize;
    }

    for (int i = 0; i < numSegments; i++) {
        pthread_create(&threads[i], NULL, processSegment, &threadData[i]);
    }

    for (int i = 0; i < numSegments; i++) {
        pthread_join(threads[i], NULL);
    }

    // Consolidate word frequency data
    WordData consolidatedData[fileSize / 2];
    int numWordsConsolidated = 0;

    for (int i = 0; i < numSegments; i++) {
        for (int j = 0; j < threadData[i].numWords; j++) {
            int foundIndex = -1;
            for (int k = 0; k < numWordsConsolidated; k++) {
                if (strcmp(threadData[i].wordData[j].word, consolidatedData[k].word) == 0) {
                    foundIndex = k;
                    break;
                }
            }

            if (foundIndex != -1) {
                consolidatedData[foundIndex].frequency += threadData[i].wordData[j].frequency;
            } else {
                int newIndex = numWordsConsolidated;
                strcpy(consolidatedData[newIndex].word, threadData[i].wordData[j].word);
                consolidatedData[newIndex].frequency = threadData[i].wordData[j].frequency;
                numWordsConsolidated++;
            }
        }
    }

    // Print the consolidated word frequency data
    for (int i = 0; i < numWordsConsolidated; i++) {
        printf("%s: %d\n", consolidatedData[i].word, consolidatedData[i].frequency);
    }

    free(fileData);

    return 0;
}

