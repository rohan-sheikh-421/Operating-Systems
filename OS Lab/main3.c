#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) 
{
    int i;
    if (argc < 2) 
    {
        printf("Please enter a number between 1 and 10 as command line argument.\n");
        return 1;
    }
    int numProcesses = atoi(argv[1]);
    if (numProcesses < 1 || numProcesses > 10) 
    {
        printf("Please enter a number between 1 and 10 as command line argument.\n");
        return 1;
    }
    pid_t pid;
    for (i=0; i < numProcesses - 1; i++) 
    {
        pid = fork();
        if (pid < 0) 
        {
            printf("child process not created.\n");
            return 1;
        } else if (pid == 0)
        {
            // Child process
            printf("Child %d: My PID is %d and my parent's PID is %d.\n", i+1, getpid(), getppid());
            return 0;
        }
    }
    printf("Child %d: My PID is %d and my parent's PID is %d.\n", numProcesses, getpid(), getppid());
    for (i = 0; i < numProcesses - 1; i++) 
    {
        wait(NULL);
    }
    return 0;
}
