#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define MAX_TASKS 10  // Maximum allowed tasks

typedef struct {
    pid_t pid;
    char command[256];  
} Task;

Task tasks[MAX_TASKS];  // Store running tasks
int taskCount = 0; 

void startTask();
void listTasks();
void stopTask();
void exitProgram();
void checkZombies();

int main() {
    int choice;
    
    while (1) {
        checkZombies();  // Cleanup terminated tasks

        printf("\n----Task Manager----\n");
        printf("1. Start Task\n");
        printf("2. List Running Tasks\n");
        printf("3. Stop Task\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: startTask(); break;
            case 2: listTasks(); break;
            case 3: stopTask(); break;
            case 4: exitProgram(); break;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

void startTask() {
		printf("TODO Start Task");
}

void listTasks() {
		printf("TODO List Tasks");
}

void stopTask() {
		printf("TODO Stop Task");
}

void exitProgram(){
		printf("TODO Exit Program");
		// Make sure to terminate all running childern processes before exiting
		}

void checkZombies() {
    printf("TODO Check Zombies");
    // Check if there are any zombie processes remove it & remove it from tasks array
 }
