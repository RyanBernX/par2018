CC=mpicc
CFLAGS=-fopenmp

.PHONY: all

all: test send

send: mpi_send.o
	${CC} -o $@ $< ${CFLAGS}

test: mpi_test.o
	${CC} -o $@ $< ${CFLAGS}

%.o: %.c
	${CC} -c -o $@ $< ${CFLAGS}

clean:
	-rm *.o send test

