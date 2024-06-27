CC=gcc
CCOPTS=--std=gnu99 -g -Wall 

BINS=context_example

.phony: clean all

all:	$(BINS)

context_example: so_exam_2019_01_22_06_ucontext.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
