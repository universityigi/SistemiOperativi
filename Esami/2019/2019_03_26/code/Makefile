CC=gcc
CCOPTS=--std=gnu99 -g -Wall 
LDOPTS=-lrt -lpthread
BINS=so_exam_2019_03_26_producer so_exam_2019_03_26_consumer

.phony: clean all

all:	$(BINS)

so_exam_2019_03_26_producer: so_exam_2019_03_26_producer.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^ $(LDOPTS)
so_exam_2019_03_26_consumer: so_exam_2019_03_26_consumer.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^ $(LDOPTS)

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
