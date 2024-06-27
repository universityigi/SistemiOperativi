CC=gcc
CCOPTS=--std=gnu99 -g -Wall 
LDOPTS=-lrt -lpthread
BINS=so_exam_2019_11_06_processes_and_threads

.phony: clean all

all:	$(BINS)

so_exam_2019_11_06_processes_and_threads: so_exam_2019_11_06_processes_and_threads.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^ $(LDOPTS)

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
