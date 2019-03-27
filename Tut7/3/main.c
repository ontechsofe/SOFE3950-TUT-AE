#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHELL "/bin/sh"

pid_t gChildPID = -1;

void killChild(int sig) {
  pid_t pd = getpid();
  printf("\n\n%d\n\n", pd);
  kill(pd, SIGKILL);
}

int main(void) {
  int status;
  signal(SIGALRM, (void (*)(int))killChild);
  gChildPID = fork();
  switch (gChildPID) {
    case 0:
      execl(SHELL, SHELL, "-c", "./process 10", NULL);
      break;
    case -1:
      status = -1;
      break;
    default:
      alarm(5);
      wait(NULL);
      break;
  }
  return status;
}