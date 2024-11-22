#include <windows.h>
#include <stdio.h>
#include <conio.h>  // For _kbhit() and _getch()

HANDLE hThread;
HANDLE hEvent;
BOOL shouldExit = FALSE;
BOOL threadShouldExit = FALSE;

DWORD WINAPI BackgroundTask(LPVOID lpParam) {
    while (!threadShouldExit) {
        Sleep(5000);  // Sleep for 5 seconds

        // Periodic task - simulate work done after each sleep cycle
        printf("Background task completed 5 seconds.\n");

        // Signal event (you could also signal the event after every task or after a condition)
        SetEvent(hEvent);  
    }
    printf("Background thread exiting...\n");
    return 0;
}

void ListenForUserInput() {
    char input[10];

    while (!shouldExit) {
        if (_kbhit()) {
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;

            if (strcmp(input, "1") == 0) {
                shouldExit = TRUE;
                threadShouldExit = TRUE;
                break;
            }
            else {
                printf("Invalid input, continuing...\n");
            }
        }

        Sleep(100); // Avoid high CPU usage
    }
}

int main() {
    DWORD threadId;

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (hEvent == NULL) {
        printf("Failed to create event\n");
        return 1;
    }

    // Create the background thread
    hThread = CreateThread(NULL, 0, BackgroundTask, NULL, 0, &threadId);
    
    if (hThread == NULL) {
        printf("Failed to create thread\n");
        return 1;
    }

    ListenForUserInput();

    if (shouldExit) {
        printf("Exiting gracefully...\n");
    }

    // Wait for the background thread to finish if it's still running
    WaitForSingleObject(hThread, INFINITE);

    // Close handles
    CloseHandle(hEvent);
    CloseHandle(hThread);

    return 0;
}
