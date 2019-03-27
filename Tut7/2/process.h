#ifndef PROCESS
#define PROCESS

typedef void* Process;
Process createProcess(char name[256], int priority, int pid, int runtime);

void printProcess(Process _process);

// Process priorities
typedef enum {
    REAL_TIME = 0,
    HIGH_USERJOB,
    MID_USERJOB,
    LOW_USERJOB
} process_priority_t;

// Structure to handle the passed processes from the file being read.
typedef struct process_st {
    char name[256];
    int priority;
    int pid;
    int runtime;
} process_t;

#endif
