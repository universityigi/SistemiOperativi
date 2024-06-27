#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 16384
#define ITERATIONS 5
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

ucontext_t main_context, f1_context, f2_context;

void f1(){
  for (int i=0; i<ITERATIONS; i++) {
    printf("f1: %d\n", i);
    swapcontext(&f1_context, &f2_context);
  }
  setcontext(&main_context);
}

void f2(){
  for (int i=0; i<ITERATIONS; i++) {
    printf("f2: %d\n", i);
    exit(0);
  }
  setcontext(&main_context);
}

char f1_stack[STACK_SIZE];
char f2_stack[STACK_SIZE];

int main() {
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

  // this passes control to f2.
  // and saves the current context in main_context
  swapcontext(&main_context, &f1_context); // we will jump back here
  printf("exiting\n");
}
