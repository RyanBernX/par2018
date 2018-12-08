CC=mpicc
TARGETS=01 02 03 04 05 06 07

.PHONY: all clean

all: ${TARGETS}

%: %.c
	${CC} -g -o $@ $<

clean:
	rm -f ${TARGETS}

