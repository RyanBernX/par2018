#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

#define N 64

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
  int i, rank;
  int * buffer;
  MPI_File fh;
  MPI_Status status;
  MPI_Errhandler errhdl;
  MPI_Init(&argc, &argv);
  MPI_Errhandler_create(&err_handler, &errhdl);
  MPI_Errhandler_set(MPI_COMM_WORLD, errhdl);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_File_open(MPI_COMM_WORLD, "tmp.dat",
		MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, 
		&fh);
  MPI_File_preallocate(fh, N*sizeof(int));
  MPI_File_set_view(fh, 0, MPI_INT, MPI_INT, 
		    "native", MPI_INFO_NULL);
  buffer = (int *)malloc(N*sizeof(int));
  //MPI_File_set_atomicity(fh, 0);
  if (rank == 0) {
    for (i = 0;i < N;i ++)
      buffer[i] = i;
    MPI_File_write_at(fh, 0, buffer, N, MPI_INT, &status);
    printf("rank %d: writing data as\n", rank);
    for (i = 0;i < N;i ++)
      printf("%d\t", buffer[i]);
    printf("\n");
  }
  else {
    MPI_File_read_at(fh, 0, buffer, N, MPI_INT, &status);
    printf("rank %d: reading data as\n", rank);
    for (i = 0;i < N;i ++)
      printf("%d\t", buffer[i]);
    printf("\n");
  }
  free(buffer);
  MPI_File_close(&fh);
  MPI_Errhandler_free(&errhdl);
  MPI_Finalize();
  return 0;
}

