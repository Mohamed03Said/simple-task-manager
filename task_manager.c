#include "task_manager.h"


Task tasks[MAX_TASKS];
int taskCount = 0;


void getChoice(int *choice) {
    printf("Enter choice number: ");
    scanf("%d", choice);
    getchar();
}


void displayTasksMenu() {
    printf("\nSelect a task to start:\n");
    printf("1. Ping google.com\n");
    printf("2. Show Date\n");
    printf("3. Sleep 10 seconds\n");
    printf("0. Back To Main Menu\n");
}

void startTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Maximum number of tasks reached.\n");
        return;
    }

    int choice = -1;

    while (choice != 0) {
        displayTasksMenu();
        getChoice(&choice);

        if (choice == 0) {
            printf("Returning to main menu...\n");
            return;
        }

        if (choice < 0 || choice > 3) {
            printf("Invalid choice, please try again.\n");
            continue;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
        } 
        else if (pid == 0) {// Child process
            switch (choice) {
                case 1:
                    execlp("ping", "ping", "-c", "4", "google.com", NULL);
                    break;
                case 2:
                    execlp("date", "date", NULL);
                    break;
                case 3:
                    execlp("sleep", "sleep", "10", NULL);
                    break;
                default:
                    printf("Unknown choice in child process.\n");
                    break;
            }
            exit(0);
        } 
        else {
            tasks[taskCount].pid = pid;
            switch (choice) {
                case 1:
                    strcpy(tasks[taskCount].command, "ping google.com");
                    break;
                case 2:
                    strcpy(tasks[taskCount].command, "date");
                    break;
                case 3:
                    strcpy(tasks[taskCount].command, "sleep 10");
                    break;
                default:
                    strcpy(tasks[taskCount].command, "unknown");
                    break;
            }
            taskCount++;
            int status;
            waitpid(pid, &status, 0);
            printf("Started task with PID %d\n", pid);
        }
    }
}

void runningTasks(){
    if (taskCount == 0) {
        printf("No tasks are currently running.\n");
        return;
    }
  for (int i = 0; i < taskCount; i++) {
        printf("Task %d: PID %d, Command: %s\n", i + 1, tasks[i].pid, tasks[i].command);
    }
    printf("\n");
}

void listTasks() {
    if(taskCount > 0){
      printf("\n---- List of Running Tasks ----\n");
    }
    runningTasks();
}

void stopTask() {
    if(taskCount == 0){
      printf("No running tasks to stop.\n");
    }
    
    runningTasks();
    
    int choice = -1;

    printf("Enter the task number to stop: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > taskCount) {
        printf("No Task with this number.");
        return;
    }

    pid_t pid = tasks[choice - 1].pid;

    if (kill(pid, SIGTERM) == 0) {
        printf("Task with PID= %d stopped successfully.\n", pid);

        for (int i = choice - 1; i < taskCount - 1; i++) {//remove form the list
            tasks[i] = tasks[i + 1];
        }
        taskCount--;
    } 
    else {
        perror("Failed to stop task");
    }
}


void exitProgram() {
    printf("\nTerminating all running tasks...\n");

    for (int i = 0; i < taskCount; i++) {
        pid_t pid = tasks[i].pid;

        if (kill(pid, SIGTERM) == 0) {
            printf("Sent SIGTERM to task with PID %d\n", pid);
        } else {
            perror("Failed to send SIGTERM");
        }

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("Error waiting for child process");
        } 
        else {
            if (WIFEXITED(status)) {
                printf("Task with PID %d exited with status %d\n", pid, WEXITSTATUS(status));
            } 
            else if (WIFSIGNALED(status)) {
                printf("Task with PID %d terminated by signal %d\n", pid, WTERMSIG(status));
            }
        }
    }
    
    taskCount = 0;
    printf("All tasks terminated and cleaned up. Exiting program...\n");
    exit(0);
}

void checkZombies() {
    for (int i = 0; i < taskCount; i++) {
        pid_t pid = tasks[i].pid;
        int status;

        pid_t result = waitpid(pid, &status, WNOHANG); //prevents blocking in the parent process

        if (result == -1) {
            perror("Error in waitpid");
        } 
        else if (result > 0) { // Child process has terminated
            if (WIFEXITED(status)) {
                printf("Task with PID %d finished with exit status %d\n", pid, WEXITSTATUS(status));
            } 
            else if (WIFSIGNALED(status)) {
                printf("Task with PID %d terminated by signal %d\n", pid, WTERMSIG(status));
            }
            
            for (int j = i; j < taskCount - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            taskCount--;
            i--;
        }
    }
}

