#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main() {
    string input;

    while (true) {
        // Prompt the user for a command
        cout << "MyCustomShell> ";
        getline(cin, input);

        // Exit the shell if the user enters "exit"
        if (input == "exit") {
            break;
        }

        // Tokenize the input into command and arguments
        vector<string> tokens;
        char* token = strtok(const_cast<char*>(input.c_str()), " ");
        while (token != nullptr) {
            tokens.push_back(token);
            token = strtok(nullptr, " ");
        }

        if (tokens.empty()) {
            cout << "Invalid command." << endl;
            continue;
        }

        // Convert vector of strings to an array of C-style strings
        char* argv[tokens.size() + 1]; // +1 for the nullptr
        for (size_t i = 0; i < tokens.size(); ++i) {
            argv[i] = const_cast<char*>(tokens[i].c_str());
        }
        argv[tokens.size()] = nullptr;

        // Create a child process
        pid_t pid = fork();

        if (pid == -1) {
            cerr << "Fork failed." << endl;
            return 1;
        } else if (pid == 0) {
            // This is the child process
            execvp(argv[0], argv);
            
            // If execvp returns, there was an error
            cerr << "Command not found: " << argv[0] << endl;
            exit(EXIT_FAILURE); // Exit the child process with a failure code
        } else {
            // This is the parent process
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int exitStatus = WEXITSTATUS(status);
                if (exitStatus == 0) {
                    cout << "Command executed successfully." << endl;
                } else {
                    cerr << "Command failed with exit code " << exitStatus << "." << endl;
                }
            } else {
                cerr << "Command did not terminate normally." << endl;
            }
        }
    }

    return 0;
}
