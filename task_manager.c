#include "task_manager.h"


Task tasks[MAX_TASKS];
int taskCount = 0;


void get_choice(int *choice) {
    printf("Enter choice number: ");
    scanf("%d", choice);
    getchar();
}


void display_tasks_menu() {
    printf("\nSelect a task to start:\n");
    printf("1. Ping google.com\n");
    printf("2. Show Date\n");
    printf("3. Sleep 10 seconds\n");
    printf("4. Display Content Of Current Directory\n");
    printf("0. Back To Main Menu\n");
}

void startTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Maximum number of tasks reached.\n");
        return;
    }

    int choice = -1;

    while (choice != 0) {
        display_tasks_menu();
        get_choice(&choice);
        if(choice == 0){
          continue;
        }
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
        } 
        else if (pid == 0) {  // Child process
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
                case 4:
                    execlp("ls", "ls", NULL);
                    break;
                default:
                    printf("Invalid Choice.\n");
                    break;
            }
            exit(0);
        } 
        else {  // Parent process
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
                case 4:
                    strcpy(tasks[taskCount].command, "ls");
                    break;
                default:
                    strcpy(tasks[taskCount].command, "unknown");
                    break;
            }
            taskCount++;

            int status;
            waitpid(pid, &status, 0);  // Wait for the child to finish
            printf("Started task with PID %d\n", pid);
        }
    }
}


void running_tasks(){
    if (taskCount == 0) {
        printf("No tasks are currently running.\n");
        return;
    }
  for (int task_number = 0; task_number < taskCount; task_number++) {
        printf("Task %d: PID %d, Command: %s\n", task_number + 1, tasks[task_number].pid, tasks[task_number].command);
    }
    printf("\n");
}

void listTasks() {
    if(taskCount > 0){
      printf("\n---- List of Running Tasks ----\n");
    }
    running_tasks();
}

bool stop_task_by_PID(pid_t pid, int signal){
  bool is_stopped = false;
  
  int killResult = kill(pid, signal);
    if (killResult == 0) {
        printf("signal sent to PID %d.\n", pid);

        int status;
        if (waitpid(pid, &status, 0) == -1) { // wait until it terminated
            perror("Error waiting for process");
        } 
        else {
            if (WIFEXITED(status)) {
                printf("Task exited with status %d\n", WEXITSTATUS(status));
            } 
            else if (WIFSIGNALED(status)) {
                printf("Task killed by signal %d\n", WTERMSIG(status));
            }
        }
        is_stopped = true;
    }
    else {
        switch (errno) {
            case ESRCH:
                printf("Process %d already exited.\n", pid);
                is_stopped = true;
                break;
            case EPERM:
                printf("No permission to kill process %d.\n", pid);
                break;
            case EINVAL:
                printf("Invalid signal passed to kill().\n");
                break;
            default:
                perror("Failed to send SIGTERM");
        }

        int status;
        if (waitpid(pid, &status, WNOHANG) > 0) { // handle it's already exited
            printf("Process %d was already exited.\n", pid);
            is_stopped = true;
        }
    }
    
    return is_stopped;
}

void clean_up(int *task_number){
  for (int i = *task_number + 1; i < taskCount - 1; i++) {//remove form the list
            tasks[i-1] = tasks[i];
        }
        taskCount--;
}

void stopTask() {
    if(taskCount == 0){
      printf("No running tasks to stop.\n");
      return;
    }
    
    running_tasks();
    
    int task_number;

    get_choice(&task_number);

    if (task_number < 1 || task_number > taskCount) {
        printf("No Task with this number.");
        return;
    }

    pid_t pid = tasks[task_number - 1].pid;

    bool result = stop_task_by_PID(pid, SIGTERM);
    
    if(result){
      printf("Task PID= %d stopped successfully.\n", pid);
      clean_up(&task_number);
    }
    else{
      printf("Failed to stop Task PID= %d stopped successfully.\n", pid);
    }
}

void exitProgram() {
    printf("\nTerminating all running tasks...\n");
    
    bool stopped_all = true;
    for (int task_number = 0; task_number < taskCount; task_number++) {
        pid_t pid = tasks[task_number].pid;
        stopped_all &= stop_task_by_PID(pid, SIGTERM);
    }
    if(stopped_all){
      taskCount = 0;
      printf("All tasks terminated and cleaned up. Exiting program...\n");
      exit(0);
    }
    else{
      printf("Failed to stop all tasks\n");
    }
}

void checkZombies() {
    printf("Check Zombies...\n");
    for (int task_number = 0; task_number < taskCount; task_number++) {
        pid_t pid = tasks[task_number].pid;
        int status;
        
        pid_t result = waitpid(pid, &status, WNOHANG); // avoid blocking the parent

        if (result == -1) {
            perror("Error in waitpid");
        } 
        else if (result > 0) { // child terminated
            if (WIFEXITED(status)) {
                printf("Task PID %d exited with status %d\n", pid, WEXITSTATUS(status));
            } 
            else if (WIFSIGNALED(status)) {
                printf("Task PID %d terminated by signal %d\n", pid, WTERMSIG(status));
            }

            clean_up(&task_number);
            task_number--;
        }
    }
}

