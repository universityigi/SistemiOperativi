#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <signal.h>

#define SHMEM_SIZE 1
#define NUM_ROUNDS 10

//! @brief sigint handler otherwise you have to reboot the damn pc every time
void sigHandler(int sig_number_);

char* sem_name_0;
char* sem_name_1;
char* resource_name;

int main(int argc, char** argv) {
  if (signal(SIGINT, sigHandler) == SIG_ERR)
    printf("error can't catch SIGINT\n");
  
  sem_name_0 = "s0";
  sem_name_1 = "s1";
  resource_name = "object";

  sem_t* sem_0 = sem_open(sem_name_0, O_CREAT, 0666, 0); //! full
  sem_t* sem_1 = sem_open(sem_name_1, O_CREAT, 0666, 1); //! empty

  int fd=shm_open(resource_name, O_RDWR|O_CREAT, 0666);
  if (fd < 0){
    printf("cannot create shared memory object, error: %s \n", strerror(errno));
    exit(-1);
  }
  int ftruncate_result = ftruncate(fd, SHMEM_SIZE);
  if (ftruncate_result < 0) {
    printf("cannot truncate shared memory object, error: %s \n", strerror(errno));
    exit(-1);
  }

  void * my_memory_area = mmap(NULL, SHMEM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
  for (int i=0; i < NUM_ROUNDS; ++i) {
    sem_wait(sem_1);
    char* buffer=(char*) my_memory_area;
    sprintf(buffer, "%d", i);
    printf("p1|%s\n", buffer);
    sem_post(sem_0);
    sleep(1);
  }

  printf("p1|exit\n");
  
  int unlink_result=shm_unlink(resource_name);
  if (unlink_result<0) {
    printf("cannot unlink shared memory object, error: %s \n", strerror(errno));
    exit(-1);
  }

  sem_close(sem_0);
  sem_close(sem_1);

  sem_unlink(sem_name_0);
  sem_unlink(sem_name_1);
  
  return 0;
}


void sigHandler(int sig_number_) {
  if (sig_number_ != SIGINT){
    printf("error, wrong signal\n");
    exit(EXIT_FAILURE);
  }
  
  printf("received SIGINT, cleaning\n");
    
  sem_unlink(sem_name_0);
  sem_unlink(sem_name_1);

  int unlink_result=shm_unlink(resource_name);
  if (unlink_result<0) {
    printf("cannot unlink shared memory object, error: %s \n", strerror(errno));
    exit(-1);
  }

  exit(EXIT_SUCCESS);
}
