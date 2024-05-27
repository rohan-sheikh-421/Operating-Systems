#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int numLaps;
int lapTime;

void printLapNumber(int sig) {
    static int lapCount = 1;
    printf("Lap %d completed\n", lapCount);
    lapCount++;

    if (lapCount > numLaps) {
        kill(getpid(), SIGTERM); // Send SIGTERM to terminate the current process
    }
}

void printTotalTime(int sig) {
    printf("Total time: %d seconds\n", numLaps * lapTime);
}

int main() {
    printf("Enter the number of laps: ");
    scanf("%d", &numLaps);

    printf("Enter the lap time (in seconds): ");
    scanf("%d", &lapTime);

    signal(SIGALRM, printLapNumber); // Install signal handler for SIGALRM
    signal(SIGTERM, printTotalTime); // Install signal handler for SIGTERM

    pid_t childPid = fork(); // Create a child process

    if (childPid == 0) {
        // Child process
        while (1) {
            alarm(lapTime); // Set alarm for the specified lap time
            pause(); // Wait for the alarm to trigger
        }
    } else if (childPid > 0) {
        // Parent process
        printf("Stopwatch started...\n");
        wait(NULL); // Wait for the child process to terminate
    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}

