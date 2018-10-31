#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

#define LEN 100000000L

int main(int argc, char **argv){
  int rank, size;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 2){
    if (rank == 0)
      fprintf(stderr, "This program is for 2 processes only. Got %d\n", size);
    MPI_Finalize();
    return 0;
  }

  double *data, t;
  data = (double*)calloc(LEN, sizeof(double));
  t = MPI_Wtime();
  //MPI_Sendrecv(data, LEN, MPI_DOUBLE, 1, 99, data, LEN, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
  if (rank == 0)
    MPI_Send(data, LEN, MPI_DOUBLE, 1, 99, MPI_COMM_WORLD);
  else
    MPI_Recv(data, LEN, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  MPI_Barrier(MPI_COMM_WORLD);
  t = MPI_Wtime() - t;

  if (rank == 0)
    printf("%ld MiB sent in %f sec.\n", 8 * LEN / 1000000, t);

  MPI_Finalize();
  return 0;
}

