#include <stdlib.h>
#include <stdio.h>
#include "processQueue.h"

ProcessQueue createProcessQueue(unsigned int capacity) {
    queue_t *processQueue;
    if (capacity <= 0) {return NULL;}
    processQueue = (queue_t *)malloc(sizeof(queue_t));
    processQueue->size = 0;
    processQueue->capacity = capacity;
    processQueue->head = NULL;
    processQueue->tail = NULL;
    return (ProcessQueue) processQueue;
}

ProcessQueueElement createProcessQueueNode(Process _process) {
    queue_element_t *queueElement;
    process_t *process = (process_t *)_process;
    queueElement = (queue_element_t *)malloc(sizeof(queue_element_t));
    queueElement->curr = process;
    queueElement->next = NULL;
    return (ProcessQueueElement) queueElement;
}

void processEnqueue(ProcessQueue _processQueue, Process _process) {
    queue_t *queue = (queue_t *)_processQueue;
    queue_element_t *queueElement = (queue_element_t *)createProcessQueueNode(_process);
    if (queue->tail == NULL) {
        queue->head = queue->tail = queueElement;
        queue->size++;
    } else {
        queue->tail->next = queueElement;
        queue->tail = queueElement;
        queue->size++;
    }
}

Process processDequeue(ProcessQueue _processQueue) {
    queue_t *queue = (queue_t *)_processQueue;
    if (queue->head == NULL) {
        return NULL;
    }
    queue->size--;
    queue_element_t *toBeDequeued = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    return (Process)toBeDequeued->curr;
}

Process getProcessQueueHead(ProcessQueue _processQueue) {
    queue_t *queue = (queue_t *)_processQueue;
    if (queue->head == NULL) {
        return NULL;
    }
    return (Process) queue->head->curr;
}

Process getProcessQueueTail(ProcessQueue _processQueue) {
    queue_t *queue = (queue_t *)_processQueue;
    return (Process) queue->tail->curr;
}

int getQueueLength(ProcessQueue _processQueue) {
    queue_t *queue = (queue_t *)_processQueue;
    return queue->size;
}

void printProcessQueue(ProcessQueue _processQueue) {
    queue_t *queue = (queue_t *)_processQueue;
    printf("Process Queue:\n");
    printf("\tcapacity: %i\n", queue->capacity);
    printf("\tsize: %i\n", queue->size);
}

void fillQueueFromFile(ProcessQueue _processQueue, char* _fileName) {
    int c = 0;
    FILE* file = fopen(_fileName, "r+");
    char line[256];
    char name[256];
    int priority, pid, runtime;
    while(fgets(line, sizeof(line), file)) {
        // printf("%s", line);
        sscanf(line, "%[^,],%d,%d,%d", &name, &priority, &pid, &runtime);
        processEnqueue(_processQueue, createProcess(name, priority, pid, runtime));
    }
    fclose(file);
}