CC=gcc
ifdef MKL
    CFLAGS=-I${MKLROOT}/lib/intel64 -DHAVE_MKL -m64
    LDFLAGS=-Wl,--no-as-needed -Wl,-rpath ${MKLROOT}/lib/intel64 -L${MKLROOT}/lib/intel64 \
            -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lm
else
    CFLAGS=-I/opt/lapack-3.8.0/include
    LDFLAGS=-L/opt/lapack-3.8.0/lib -llapacke -lcblas -llapack -lblas -lgfortran -lm
endif

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
