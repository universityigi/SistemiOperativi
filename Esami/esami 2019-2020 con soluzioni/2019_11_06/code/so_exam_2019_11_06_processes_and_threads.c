#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_STEPS 5
unsigned int value = 0;
pid_t pid;
pthread_t tid;

void* runner(void* param); 

int main(int argc, char *argv[]) {
  pthread_attr_t attr;
  pid = fork();
  
  if (pid < 0)
    return -1;
  
  if (pid == 0) {
    pthread_attr_init(&attr);
    pthread_create(&tid,&attr,runner,NULL);
    pthread_join(tid,NULL);
    printf("line C, value = %d\n",value); /* LINE C */
  } else {
    wait(NULL);
    printf("line P, value = %d\n",value); /* LINE P */
  }
  return 0;
}

void* runner(void* param) {
  for (int s = 0; s < NUM_STEPS; ++s) {
    if (!pid) {
      value++;
    }
  }
  return param;
}
