all: run.exe

run.exe: list.h list.c example.c
	gcc -g -std=c99 -lm list.c example.c -o run.exe

.PHONY: clean

clean:
	rm -f *.out *.exe *.o vgcore.*

