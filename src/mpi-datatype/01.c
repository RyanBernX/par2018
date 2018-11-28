#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char * argv[])
{
  int i, j, k, rank;
  int ** buffer;
  char file[128];
  FILE * fp;
  MPI_Status * status;
  MPI_Request * request;

  MPI_Init(&argc, &argv);
  int n_buffer = atoi(argv[1]);
  int buffer_size = atoi(argv[2]);

  assert(n_buffer > 0);
  buffer = (int **)malloc(n_buffer*sizeof(int *));
  for (i = 0;i < n_buffer;i ++)
    buffer[i] = (int *)malloc(buffer_size*sizeof(int));

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  status = (MPI_Status *)malloc(n_buffer*sizeof(MPI_Status));
  request = (MPI_Request *)malloc(n_buffer*sizeof(MPI_Request));
  if (rank == 0) {
    for (i = 0;i < n_buffer;i ++) {
      buffer[i][0] = i;
      MPI_Isend(buffer[i], buffer_size, MPI_INT, 
		1, i + 99, MPI_COMM_WORLD, &request[i]);
    }
  }
  else {
    for (i = 0;i < n_buffer;i ++) {
      MPI_Irecv(buffer[i], buffer_size, MPI_INT, 
		0, i + 99, MPI_COMM_WORLD, &request[i]);
    }
  }
  MPI_Waitall(n_buffer, request, status);
  free(request);
  free(status);
  MPI_Finalize();

  for (i = 0;i < n_buffer;i ++) {
    sprintf(file, "data_%d_%d.dat", rank, i);
    fp = (FILE *)fopen(file, "wb");
    fwrite(buffer[i], sizeof(int), buffer_size, fp);
    fclose(fp);
    free(buffer[i]);
  }
  free(buffer);
  return 0;
}

