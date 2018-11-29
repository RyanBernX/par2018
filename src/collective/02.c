#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROOT 3

int main(int argc, char * argv[])
{
  int i, rank, n_rank;
  int * send_buffer;
  int * recv_buffer;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
  send_buffer = (int *)calloc(1, sizeof(int));
  send_buffer[0] = rank;
  if (rank == ROOT) {
    recv_buffer = (int *)calloc(n_rank, sizeof(int));
    for (i = 0;i < n_rank;i ++)
      printf("recv_buffer[%d] = %d\n", i, recv_buffer[i]);
  }
  MPI_Gather(send_buffer, 1, MPI_INT, 
	     recv_buffer, 1, MPI_INT, 
	     ROOT, MPI_COMM_WORLD);
  if (rank == ROOT) {
    for (i = 0;i < n_rank;i ++)
      printf("recv_buffer[%d] = %d\n", i, recv_buffer[i]);
    free(recv_buffer);
  }
  free(send_buffer);
  MPI_Finalize();
  return 0;
}

