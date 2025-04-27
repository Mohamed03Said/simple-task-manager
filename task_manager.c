#inclue "task_manager.h"


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
            printf("Started task with PID %d\n", pid);
        }
    }
}


