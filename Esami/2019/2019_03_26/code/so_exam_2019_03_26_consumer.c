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

//! @brief sigint handler otherwise you have to reboot the damn pc every time
void sigHandler(int sig_number_);

sem_t* sem_0 = NULL;
sem_t* sem_1 = NULL;
char* resource_name;

int main(int argc, char** argv) {
  if (signal(SIGINT, sigHandler) == SIG_ERR)
    printf("error can't catch SIGINT\n");

  char* sem_name_0 = "s0";
  char* sem_name_1 = "s1";
  resource_name = "object";

  sem_0 = sem_open(sem_name_0, O_CREAT, 0666, 0); //! full
  sem_1 = sem_open(sem_name_1, O_CREAT, 0666, 1); //! empty
  
  int fd = shm_open(resource_name, O_RDONLY, 0666);
  if (fd < 0){
    printf("cannot link to shared memory object, error: %s \n", strerror(errno));
    exit(-1);
  }
  
  int SHMEM_SIZE = 0;

  struct stat shm_status;
  int fstat_result = fstat(fd, &shm_status);
  if (fstat_result < 0){
    printf("cannot get stats from memory object, error: %s \n", strerror(errno));
    exit(-1);
  }
  
  SHMEM_SIZE = shm_status.st_size;
  
  void* my_memory_area = mmap(NULL, SHMEM_SIZE, PROT_READ, MAP_SHARED, fd, 0);

  while (1) {
    sem_wait(sem_0);
    int n = atoi((char*) my_memory_area);
    printf("p2|%d\n", n);
    sem_post(sem_1);
    if (4 == n)
      break;
  }

  printf("p2|exit\n");
  
  int unlink_result = shm_unlink(resource_name);
  if (unlink_result < 0) {
    printf("cannot unlink shared memory object, error: %s \n", strerror(errno));
    exit(-1);
  }

  sem_close(sem_0);
  sem_close(sem_1);
  
  return 0;
}


void sigHandler(int sig_number_) {
  if (sig_number_ != SIGINT){
    printf("error, wrong signal\n");
    exit(EXIT_FAILURE);
  }
  
  printf("received SIGINT, cleaning\n");
    
  sem_close(sem_0);
  sem_close(sem_1);

  exit(EXIT_SUCCESS);
}
