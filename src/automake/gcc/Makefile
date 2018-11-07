hello : hello.o hello_fn.o

hello.o : hello.c hello.h
	cc -c hello.c

hello_fn.o : hello_fn.o hello.h
	cc -c hello_fn.c

clean : 
	-rm -f hello hello.o hello_fn.o
