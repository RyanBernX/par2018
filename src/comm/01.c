#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DIM 2
#define true 1
#define false 0

int n_process;			// number of process started
int rank;			// rank of this process
int n_dimension_process[DIM];	// number of process in every dimension
MPI_Comm MPI_COMM_CART;		// cardinate communicator
int cart_coord[DIM];
int neighbour_process[2][DIM];	// neighbour process of this one
int periods[DIM];

int main(int argc, char * argv[])
{
  int i, j, k, i1, j1, k1, flag;

  // initialize MPI environment and build the grid topologic structure
  // of the communicator
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &n_process);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  for (i = 0;i < DIM;i ++) {
    n_dimension_process[i] = 0;
    periods[i] = false;
  }
  MPI_Dims_create(n_process, DIM, n_dimension_process);
  for (i = 0, n_process = 1;i < DIM;i ++)
    n_process *= n_dimension_process[i];
  if (rank == 0) {
    printf("Dimension arranged as: ");
    for (i = 0;i < DIM;i ++)
      printf("%d\t", n_dimension_process[i]);
    printf("\nTotal %d process under using.\n", n_process);
  }
  MPI_Cart_create(MPI_COMM_WORLD, DIM, n_dimension_process, 
		  periods, true, &MPI_COMM_CART);
  MPI_Comm_rank(MPI_COMM_CART, &rank);
  MPI_Cart_coords(MPI_COMM_CART, rank, DIM, cart_coord);
  printf("\trank %d <---> (", rank);
  for (i = 0;i < DIM;i ++)
    printf(" %d ", cart_coord[i]);
  printf(")\n");
  printf("rank %d: \n", rank);
  for (i = 0;i < DIM;i ++) {
    MPI_Cart_shift(MPI_COMM_CART, i, 1, &neighbour_process[0][i], 
		   &neighbour_process[1][i]);
    printf ("\tdim %d: up neighbour = %d, down neighbour = %d\n", 
	    i,
	    neighbour_process[0][i], 
	    neighbour_process[1][i]);
  }
  MPI_Comm_free(&MPI_COMM_CART);
  MPI_Finalize();

  return 0;
}

