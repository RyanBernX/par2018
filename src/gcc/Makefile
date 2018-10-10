all: run.exe

run.exe: inc/list.h inc/list.c example.c
	gcc -g -std=c99 -lm -I./inc/ inc/list.c example.c -o run.exe
	#gcc -O2 -std=c99 -lm list.c example.c -o run.exe

.PHONY: clean

clean:
	rm -f *.out *.exe *.o vgcore.* *.txt

