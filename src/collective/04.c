#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char * argv[])
{
  int i, rank, n_rank;
  int * send_buffer;
  int * recv_buffer;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
  if (rank == 0) {
    send_buffer = (int *)calloc(n_rank, sizeof(int));
    for (i = 0;i < n_rank;i ++)
      send_buffer[i] = i*i;
  }
  recv_buffer = (int *)calloc(1, sizeof(int));
  MPI_Scatter(send_buffer, 1, MPI_INT, 
	      recv_buffer, 1, MPI_INT, 
	      0, MPI_COMM_WORLD);
  printf("rank %d: recv_buffer[0] = %d\n", rank, recv_buffer[0]);
  free(recv_buffer);
  if (rank == 0)
    free(send_buffer);
  MPI_Finalize();
  return 0;
}

