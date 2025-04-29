#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>


#define MAX_TASKS 100


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

