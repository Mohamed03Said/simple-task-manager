#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <sys/types.h>

#define MAX_TASKS 10


typedef struct {
    pid_t pid;
    char command[256];
} Task;

extern Task tasks[MAX_TASKS];
extern int taskCount;

void startTask();
void listTasks();
void stopTask();
void exitProgram();
void checkZombies();

#endif

