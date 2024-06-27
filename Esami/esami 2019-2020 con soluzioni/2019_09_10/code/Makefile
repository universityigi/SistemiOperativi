CC=gcc
CCOPTS=--std=gnu99 -g -Wall 
LDOPTS=-lrt -lpthread
BINS=so_exam_2019_09_10_processes

.phony: clean all

all:	$(BINS)

so_exam_2019_09_10_processes: so_exam_2019_09_10_processes.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^ $(LDOPTS)

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
