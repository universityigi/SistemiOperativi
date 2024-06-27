#include <ucontext.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
  Context functions

  1.
  int getcontext(ucontext_t *ucp);
  Saves the current context in ucp. 
  
  A subsequent call to setcontext(ucp) will result in the flow
  of the program continuing from the instruction following setcontext(ucp);

  2.
  int setcontext(const ucontext_t *ucp)
  Sets the current context to ucp, a context that was previously saved.
  the flow will continue from the instruction following the
  setcontext(ucp) call issued when SAVING the context

  3.
  void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
  creates a trampoline for function func.
  func is initialized to belong to the context ucp,
  that should have the stack and the signal mask already set
  
  subsequent jumps to the context ucp will result in the flow
  of the program continuing with func.
  
  4.
  int swapcontext(ucontext_t *oucp, const ucontext_t *ucp);
  saves the current context in oucp, and jumps to ucp

*/

#define STACK_SIZE 16384
#define ITERATIONS 4
ucontext_t main_context, f1_context, f2_context;

void f1(void);
void f2(void);

char f1_stack[STACK_SIZE];
char f2_stack[STACK_SIZE];
unsigned int cnt = 0;

int main() {
  printf("- program start -\n");
  
  //get a context from main
  getcontext(&f1_context);

  // set the stack of f1 to the right place
  f1_context.uc_stack.ss_sp=f1_stack;
  f1_context.uc_stack.ss_size = STACK_SIZE;
  f1_context.uc_stack.ss_flags = 0;
  f1_context.uc_link=&main_context;

  // create a trampoline for the first function
  makecontext(&f1_context, f1, 0, 0);

  // always remember to initialize
  // a new context from something known
  f2_context=f1_context;
  
  f2_context.uc_stack.ss_sp=f2_stack;
  f2_context.uc_stack.ss_size = STACK_SIZE;
  f2_context.uc_stack.ss_flags = 0;
  f2_context.uc_link=&main_context;

  // create a trampoline for the second function
  makecontext(&f2_context, f2, 0, 0);

  // this passes control to f1
  // and saves the current context in main_context
  swapcontext(&main_context, &f1_context); // we will jump back here

  printf("- program end -\n");
}


void f1(void) {
  printf("f1: start\n");

  //ia swapping context with f2 -> f1_context now is set here
  swapcontext(&f1_context, &f2_context);

  if (cnt < ITERATIONS) {
    printf("f1: %d\n", cnt++);
    //ia doing set instead of swap, we do not update the context
    setcontext(&f1_context);
  }
  
  printf("f1: end\n");
  setcontext(&main_context);
}

void f2(void) {
  printf("f2: start\n");

  //ia swapping context with f1 -> f2_context now is set here
  getcontext(&f2_context);
  
  if (cnt < ITERATIONS/2) {
    printf("f2: %d\n", cnt++);
    //ia doing set instead of swap, we do not update the context
    setcontext(&f2_context);
  }

  printf("f2: end\n");
  swapcontext(&f2_context, &f1_context);
}
