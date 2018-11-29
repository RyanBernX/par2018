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
  send_buffer = (int *)calloc(n_rank, sizeof(int));
  for (i = 0;i < n_rank;i ++)
    send_buffer[i] = rank*n_rank + i;
  for (i = 0;i < n_rank;i ++)
    printf("rank %d: send_buffer[%d] = %d\n", rank, i, send_buffer[i]);
  recv_buffer = (int *)calloc(n_rank, sizeof(int));
  MPI_Alltoall(send_buffer, 1, MPI_INT, 
	       recv_buffer, 1, MPI_INT, 
	       MPI_COMM_WORLD);
  for (i = 0;i < n_rank;i ++)
    printf("rank %d: recv_buffer[%d] = %d\n", rank, i, recv_buffer[i]);
  free(recv_buffer);
  free(send_buffer);
  MPI_Finalize();
  return 0;
}

