CC=mpicc
CFLAGS=-g

.PHONY: all clean

all: 01 04

01: 01.c
	${CC} -o $@ $< ${CFLAGS}

04: 04.c
	${CC} -o $@ $< ${CFLAGS}

clean:
	rm -f 01 04 *.dat

