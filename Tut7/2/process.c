#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "process.h"

Process createProcess(char name[256], int priority, int pid, int runtime) {
    process_t *process;
    process = (process_t *)malloc(sizeof(process_t));
    strcpy(process->name, name);
    process->priority = priority;
    process->pid = pid;
    process->runtime = runtime;
    return (Process)process;
}

void printProcess(Process _process) {
    process_t *process = (process_t *)_process;
    printf("Process:\n");
    printf("\tName: %s\n", process->name);
    printf("\tPriority: %d\n", process->priority);
    printf("\tPID: %d\n", process->pid);
    printf("\tRuntime: %d\n", process->runtime);
}