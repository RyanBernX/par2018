CC=mpicc
CXX=mpicxx
TARGETS=01 02 03 04 06 07 08 09 th sig

.PHONY: all clean

all: ${TARGETS}
07: 07.c
	${CC} -g -o $@ $< -lpthread

08: 08.c
	${CXX} -g -o $@ $<

09: 09.c
	${CXX} -g -o $@ $<

th: th.c
	gcc -g -o $@ $< -lpthread

sig: sig.c
	gcc -g -o $@ $<

%: %.c
	${CC} -g -o $@ $<

clean:
	rm -f ${TARGETS} tmp.dat

