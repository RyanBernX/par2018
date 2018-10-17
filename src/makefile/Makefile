objects = main.o command.o

sum : $(objects)
	gcc -o sum $(objects)

main.o : defs.h
command.o : defs.h

.PHONY : clean
clean : 
	-rm sum $(objects)
