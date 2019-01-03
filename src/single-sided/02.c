#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

#define N 6

void err_handler(MPI_Comm * comm, int * err_code, ...)
{
  int resultlen;
  char err_string[MPI_MAX_ERROR_STRING];
  MPI_Error_string(*err_code, err_string, &resultlen);
  printf("MPI exception caughted: error code %d\n", *err_code);
  printf("\terror: %s\n", err_string);
  MPI_Abort(*comm, *err_code);
}

int main(int argc, char * argv[])
{
  int i = 0, j, rank, n_rank;
  int * buffer;
  MPI_Group group, root_group, else_group;
  MPI_Win window;
  MPI_Errhandler errhdl;
  MPI_Init(&argc, &argv);
  MPI_Errhandler_create(&err_handler, &errhdl);
  MPI_Errhandler_set(MPI_COMM_WORLD, errhdl);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
  MPI_Comm_group(MPI_COMM_WORLD, &group);
  MPI_Group_incl(group, 1, &i, &root_group);
  MPI_Group_difference(group, root_group, &else_group);
  if (rank == 0) {
    buffer = (int *)calloc(N*(n_rank - 1), sizeof(int));
    MPI_Win_create(buffer, N*(n_rank - 1)*sizeof(int), sizeof(int), MPI_INFO_NULL,
		   MPI_COMM_WORLD, &window);
    MPI_Win_post(else_group, 0, window);
    MPI_Win_wait(window);
    for (j = 1;j < n_rank;j ++) {
      printf("rank %d: the data put by rank %d is as\n", rank, j);
      for (i = 0;i < N;i ++) {
	printf("%d\t", buffer[(j - 1)*N + i]);
      }
      printf("\n");
    }
  }
  else {
    buffer = (int *)calloc(N, sizeof(int));
    for (i = 0;i < N;i ++)
      buffer[i] = (rank - 1)*n_rank + i;
    MPI_Win_create(buffer, 0, sizeof(int), MPI_INFO_NULL,
		   MPI_COMM_WORLD, &window);
    MPI_Win_start(root_group, 0, window);
    MPI_Put(buffer, N, MPI_INT, // data to be put
	    0, // window of this rank
	    (rank - 1)*N/2, N, MPI_INT, // where the data to be put
	    window);
    MPI_Win_complete(window);
  }
  MPI_Win_free(&window);
  MPI_Group_free(&group);
  MPI_Group_free(&root_group);
  MPI_Group_free(&else_group);
  free(buffer);
  MPI_Errhandler_free(&errhdl);
  MPI_Finalize();
  return 0;
}

