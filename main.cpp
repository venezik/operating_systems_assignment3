#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <climits>

using namespace std;

int main() {
    int arr[20];
    int start, end;
    int arrsize = sizeof(arr) / sizeof(int);
    
    srand(time(0));
    cout << "Generated array: ";
    for (int i = 0; i < arrsize; i++) {
        arr[i] = rand() % 100 + 1; // Random numbers between 1 and 100
        cout << arr[i] << " ";
    }
    cout << std::endl;

    int fd[2]; // Pipe file descriptors

    if (pipe(fd) == -1) {
        cerr << "Pipe creation failed" << std::endl;
        return 1;
    }

    int id = fork();
    if (id == -1) {
        cerr << "Fork failed" << std::endl;
        return 2;
    }

    if (id == 0) { // Child process
        start = arrsize / 2;
        end = arrsize;
        cout << "Child process loop from index " << start << " to " << end - 1 << std::endl;
    } else { // Parent process
        start = 0;
        end = arrsize / 2;
        cout << "Parent process loop from index " << start << " to " << end - 1 << std::endl;
    }

    // Finding minimum value in the assigned half
    int minVal = INT_MAX;
    for (int i = start; i < end; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }

    std::cout << "Calculated minimum: " << minVal << std::endl;

    if (id == 0) { // Child process
        cout << "Child's process ID: " << getpid() << std::endl;
        write(fd[1], &minVal, sizeof(minVal));
        close(fd[1]);
        exit(0); // Ensure child exits
    } else { // Parent process
        cout << "Parent's process ID: " << getpid() << std::endl;
        close(fd[1]); // Close unused write end
        int childMin;
        read(fd[0], &childMin, sizeof(childMin));
        close(fd[0]); // Close read end after use
        wait(NULL); // Wait for child to finish

        int overallMin = std::min(minVal, childMin);
        cout << "Parent Minimum: " << minVal << std::endl;
        cout << "Child Minimum: " << childMin << std::endl;
        cout << "Overall Minimum in the Array: " << overallMin << std::endl;
    }

    return 0;
}
