CC=mpicc
EXE=check_status ring ring_revised

.PHONY: all clean

all: $(EXE)

%: %.c
	$(CC) -o $@ $<

clean:
	rm -f $(EXE)

