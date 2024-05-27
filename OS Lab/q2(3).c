#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

int main() {
    // Fork a child process
pid_t child1 = fork();

if (child1 == 0) {
    // Child process 1
    execlp("mkdir", "mkdir", "demo_folder", NULL);
    perror("execlp");
    exit(1);
} else {
    // Parent process
    wait(NULL); // Wait for child 1 to complete
}


    // list files
   
pid_t child2 = fork();

if (child2 == 0) {
    // Child process 2
    char *touch_args[] = {"touch", "demo_folder/file1.txt", "demo_folder/file2.txt", NULL};
    execvp("touch", touch_args);
    perror("execvp");
    exit(1);
} else {
    // Parent process
    wait(NULL); // Wait for child 2 to complete
}

// Fork a child process
pid_t child3 = fork();

if (child3 == 0) {
    // Child process 3
    char *ls_args[] = {"ls", "demo_folder", NULL};
    execvp("ls", ls_args);
    perror("execvp");
    exit(1);
} else {
    // Parent process
    wait(NULL); // Wait for child 3 to complete
}


// Fork a child process
pid_t child4 = fork();

if (child4 == 0) {
    // Child process 4
    char *rm_args[] = {"rm", "-rf", "demo_folder", NULL};
    execvp("rm", rm_args);
    fprint("files deleted\n")
    perror("execvp");
    exit(1);
} else {
    // Parent process
    wait(NULL); // Wait for child 4 to complete
}
  
return 0;
}  
