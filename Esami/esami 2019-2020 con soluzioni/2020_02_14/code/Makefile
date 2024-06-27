CC=gcc
CCOPTS=--std=gnu99 -g -Wall 
LDOPTS=-lrt -lpthread
BINS=so_exam_2020_02_14_ucontext

.phony: clean all

all:	$(BINS)

so_exam_2020_01_22_ucontext: so_exam_2020_02_14_ucontext.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^ $(LDOPTS)

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
