#include <stdio.h>
#include "task_manager.h"

int main() {
    int choice = -1;

    while (choice != 4) {
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
            case 1: 
              startTask(); 
              break;
            case 2: 
              listTasks(); 
              break;
            case 3: 
              stopTask(); 
              break;
            case 4: 
              exitProgram(); 
              break;
            default: 
              printf("Invalid choice.\n");
              break;
        }
    }
}

