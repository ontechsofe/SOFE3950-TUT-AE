#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "process.h"
#include "processQueue.h"

int main(void) {
    ProcessQueue processQueue;
    processQueue = createProcessQueue(100);
    fillQueueFromFile(processQueue, "processes.txt");
    printProcessQueue(processQueue);
    while (getQueueLength(processQueue) != 0) {
        printProcess(processDequeue(processQueue));
    }
    return 0;
}