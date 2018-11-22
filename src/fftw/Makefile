CC=gcc
CFLAGS=
LIBS1=-lfftw3 -lm
LIBS2=-lfftw3_threads -lfftw3 -lm -lpthread

.PHONY: all clean

all: test1 test2

test1: test1.c
	${CC} -o $@ $< ${LIBS1} ${CFLAGS}

test2: test2.c
	${CC} -o $@ $< ${LIBS2} ${CFLAGS}

clean:
	rm -f test1 test2
