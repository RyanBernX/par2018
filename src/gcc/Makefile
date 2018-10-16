all: run.exe

# -Werror will stop the compilation if warnnings reported

run.exe: inc/list.h inc/list.c example.c
	gcc -o run.exe -g -Wall -std=c99 inc/list.c example.c -I./inc/ -lm
	#gcc -o run.exe -O2 -Wall -std=c99 -I./inc/ inc/list.c example.c -lm

lib:
	gcc -o inc/list.o -g -Wall -c -std=c99 inc/list.c
	ar crv inc/liblist.a inc/list.o
	#to produce segment-independent code
	gcc -o inc/liblist.so -Wall --shared -fPIC inc/list.c
	#-fPIC always works(but may generate larger file), but -fpic may fail on some platforms
	#gcc -o inc/liblist.so -Wall --shared -fpic inc/list.c
	#gcc -o run.exe -Wall -std=c99 example.c -I./inc -L./inc -lm -llist
	#gcc -o run.exe -Wall -std=c99 example.c -I./inc -L./inc -lm -llist -Wl,--rpath=./inc/
	gcc -o run.exe -Wall -std=c99 example.c -I./inc -L./inc -lm -llist -Wl,--rpath=./inc/,--enable-new-dtags 
	# To run, the ld.so path must be modified via LD_LIBRARY_PATH or /etc/ld.so.cache

.PHONY: clean

clean:
	rm -f *.out *.exe *.o vgcore.* *.txt inc/*.{so,a,i,s,o} *.{so,a,i,s,o}

