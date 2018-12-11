CC=gcc
CFLAGS=-I/opt/lapack-3.8.0/include
LDFLAGS=-L/opt/lapack-3.8.0/lib -llapacke -lcblas -llapack -lblas -lgomp -lgfortran -lm

multiply: multiply.c
	${CC} -O3 -o $@ $< ${LDFLAGS} ${CFLAGS}

