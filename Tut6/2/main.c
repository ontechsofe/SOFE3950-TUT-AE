#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    int pid;
    int got_pid, status;
    pid = fork();
    switch(pid) {
        case 0:
            //child
            sleep(1);
            printf("Child Process\n");
            break;
        case -1:
            perror("OOPSIE WOOPSIE!! Uwu We made a fucky wucky!! A wittle fucko boingo! The code monkeys at our headquarters are working VEWY HAWD to fix this!");
            break;
        default:
            while (got_pid = waitpid(pid, &status, 0)) {
                if ((got_pid != -1) || (errno != EINTR)) {
                    printf("Parent Process\n");
                    printf("\nChild exited with code: %d\n", status);
                    break;
                }
            }
            break;
    }
    return 0;
}

