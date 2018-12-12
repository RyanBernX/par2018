CC=gcc
CFLAGS=-I/opt/lapack-3.8.0/include
LDFLAGS=-L/opt/lapack-3.8.0/lib -llapacke -lcblas -llapack -lblas -lgomp -lgfortran -lm

.PHONY: all clean

all: multiply admm cheby

multiply: multiply.c
	${CC} -O3 -o $@ $< ${LDFLAGS} ${CFLAGS}

admm: lasso_admm_dual.c
	${CC} -O3 -o $@ $< ${LDFLAGS} ${CFLAGS}

cheby: cheby.c
	${CC} -O3 -o $@ $< ${LDFLAGS} ${CFLAGS}

clean:
	rm -f multiply admm cheby
