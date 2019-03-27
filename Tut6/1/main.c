#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER 256

void writeToFile(const char* file, char* toWrite) {
    FILE* fp;
    fp = fopen(file, "w+");
    fprintf(fp, "%s", toWrite);
    fclose(fp);
}

void readFile(const char* file) {
    char buff[BUFFER];
    FILE* fp;
    fp = fopen(file, "r+");
    while (fgets(buff, sizeof(buff), fp)) {
        printf("%s\n", buff); 
    }
    fclose(fp);
}

int main() {
    pid_t child1, child2;

    child1 = fork();
    if (child1 == 0) {
        // Child 1
        writeToFile("child1.txt", "child 1");
        sleep(1);
        readFile("child2.txt");
    } else {
        child2 = fork();
        if (child2 == 0) {
            // Child 2
            writeToFile("child2.txt", "child 2");
            sleep(1);
            readFile("child1.txt");
        } else {
            wait();
            printf("DONE\n");
        }
    }
    return 0;
}