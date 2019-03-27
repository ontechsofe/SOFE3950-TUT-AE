#ifndef PROCESS_QUEUE
#define PROCESS_QUEUE

#include "process.h"

typedef void* ProcessQueue;
typedef void* ProcessQueueElement;
ProcessQueue createProcessQueue(unsigned int capacity);
ProcessQueueElement createProcessQueueNode(Process _process);

void processEnqueue(ProcessQueue _processQueue, Process _process);
Process processDequeue(ProcessQueue _processQueue);
Process getProcessQueueHead(ProcessQueue _processQueue);
Process getProcessQueueTail(ProcessQueue _processQueue);
int getQueueLength(ProcessQueue _processQueue);
int isQueueFull(ProcessQueue _processQueue);
int isQueueEmpty(ProcessQueue _processQueue);

void printProcessQueue(ProcessQueue _processQueue);

void fillQueueFromFile(ProcessQueue _processQueue, char* _fileName);

// Struct for nodes in the linked list
typedef struct queue_element_st {
    process_t *curr;
    struct queue_element_st *next;
} queue_element_t;

// Basic Queue structure to allow for the functional arrangement of the processes on a FIFO Order
typedef struct queue_st {
    queue_element_t *head;
    queue_element_t *tail;
    int size;
    unsigned capacity;
} queue_t;


#endif
