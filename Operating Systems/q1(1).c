#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

class Stack {
private:
    int* a; // array for stack
    int max; // max size of array
    int top; // stack top
    semaphore s;
    semaphore stackNotEmpty;

public:
    Stack(int m) {
        a = new int[m];
        max = m;
        top = 0;
        s = 1; // binary semaphore
        stackNotEmpty = 0; // initially the stack is empty
    }

    void push(int x) {
        wait(s); // wait for access to the stack
        if (top == max) {
            signal(s); // release the semaphore
            wait(stackNotEmpty); // wait until the stack is not full
            wait(s); // reacquire the semaphore
        }
        a[top] = x;
        ++top;
        signal(s); // release the semaphore
    }

    int pop() {
        wait(s); // wait for access to the stack
        if (top == 0) {
            signal(s); // release the semaphore
            // wait for the stack to be not empty
            wait(stackNotEmpty);
            wait(s); // reacquire the semaphore
        }
        --top;
        int tmp = a[top];
        if (top == 0) {
            signal(stackNotEmpty); // signal that the stack is not empty
        }
        signal(s); // release the semaphore
        return tmp;
    }
};

