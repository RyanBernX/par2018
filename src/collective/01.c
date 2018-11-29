#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char * argv[])
{
  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  sleep(2*rank);
  printf("rank %d: arrive the barrier, waiting ...\n", rank);
  MPI_Barrier(MPI_COMM_WORLD);
  printf("rank %d: after the barrier\n", rank);
  MPI_Finalize();
  return 0;
}

