CC=mpicc
CXX=mpicxx
TARGETS=01 02 03 04 06 07 08 09

.PHONY: all clean

all: ${TARGETS}

08: 08.c
	${CXX} -g -o $@ $<

09: 09.c
	${CXX} -g -o $@ $<

%: %.c
	${CC} -g -o $@ $< -lpthread

clean:
	rm -f ${TARGETS}

