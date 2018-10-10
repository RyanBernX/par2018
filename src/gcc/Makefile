all: run.exe

run.exe: list.h example.c
	gcc -g -std=c99 -lm example.c -o run.exe

.PHONY: clean

clean:
	rm -f *.out *.exe *.o vgcore.*

