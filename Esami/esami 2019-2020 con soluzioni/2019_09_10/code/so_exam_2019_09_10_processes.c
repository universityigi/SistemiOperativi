#include <ucontext.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
  if (fork() == 0) { 
    if (fork() == 0) { 
      printf("A");
      return 0;
    } else {
      wait(NULL);
      printf("B");
    }
  } else {
    if (fork() == 0) { 
      printf("C");
      exit(EXIT_SUCCESS);
    } else {
      wait(NULL);
    }
    wait(NULL);
    printf("D");
  }
  
  printf("E");
  return 0;
}
