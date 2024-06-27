CC=gcc
CCOPTS=--std=gnu99 -g -Wall 

BINS=so_exam_2019_02_15_processes

.phony: clean all

all:	$(BINS)

so_exam_2019_02_15_processes: so_exam_2019_02_15_processes.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
