all: run.exe

run.exe: inc/list.h inc/list.c example.c
	gcc -g -std=c99 -lm -I./inc/ inc/list.c example.c -o run.exe
	#gcc -O2 -std=c99 -lm list.c example.c -o run.exe

lib:
	gcc -g -c -std=c99 inc/list.c -o inc/list.o
	ar crv inc/liblist.a inc/list.o
	gcc --shared -fpic -o inc/liblist.so inc/list.c
	gcc -std=c99 -I./inc -lm  -L./inc -llist example.c -o run.exe 
	# To run, the ld.so path must be modified via LD_LIBRARY_PATH or /etc/ld.so.cache

.PHONY: clean

clean:
	rm -f *.out *.exe *.o vgcore.* *.txt

