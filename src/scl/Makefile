MPICXX=$(MPIHOME)/bin/mpicxx

OBJ=main.o scl.o

%.o : %.cpp
	$(MPICXX) -g -c $< -O2 -I/usr/local/include/deal.II -fpermissive

main : $(OBJ)
	$(MPICXX) -o $@ $+ -lAFEPack_mpi -lAFEPack -lboost_serialization -llac -lbase -ldl -pthread -L$(HOME)/lib

clean :
	-rm $(OBJ) main *~ 
