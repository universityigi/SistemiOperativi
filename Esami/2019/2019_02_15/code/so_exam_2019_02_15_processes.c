#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define A_STEPS 2
#define B_STEPS 5

const char name_0[] = "A|"; 
const char name_1[] = "B|";
const char* name = name_0;

void fn0() {
  for (unsigned int i = 0; i < A_STEPS; i++) {
    printf("%s round: #%d \n", name, i);
    sleep(1);
  }
}

void fn1() {
  for (unsigned int i = 0; i < B_STEPS; i++) {
    printf("%s round: #%d \n", name, i);
    sleep(1);
  }
}

int main(int argc, char** argv) {
  printf("hello!\n");
  int f = fork();

  if (f < 0) {
    printf("%s exit", name);
    exit(EXIT_FAILURE);
  }
  
  if(0 == f) {
    name = name_1;
    fn1();
  } else {
    fn0();
  }

  printf("see ya!\n");
  exit(EXIT_SUCCESS);
}
