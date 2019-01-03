/**
 * MPI_Win_lock is not implemented in LAM 6.5.6 yet that this code can't be
 * compiled on Redhat Linux 8.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

//#define MPI_MODE_NOPRECEDE 0
//#define MPI_MODE_NOPUT 0
//#define MPI_MODE_NOSTORE 0
//#define MPI_MODE_NOSUCCEED 0

#define PUT_AT_FIRST
#define N 16

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
  MPI_Win window;
  MPI_Errhandler errhdl;
  MPI_Init(&argc, &argv);
  MPI_Errhandler_create(&err_handler, &errhdl);
  MPI_Errhandler_set(MPI_COMM_WORLD, errhdl);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    buffer = (int *)calloc(1024, sizeof(int));
    MPI_Win_create(buffer, 1024*sizeof(int), sizeof(int), MPI_INFO_NULL,
		   MPI_COMM_WORLD, &window);
    //MPI_Barrier(MPI_COMM_WORLD);
  }
  else {
    buffer = (int *)calloc(N, sizeof(int));
    MPI_Win_create(buffer, 0, sizeof(int), MPI_INFO_NULL,
		   MPI_COMM_WORLD, &window);
    for (i = 0;i < N;i ++)
      buffer[i] = i;
    if (rank == 1) {
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, window);
      printf("rank %d: get the lock\n", rank);
      MPI_Put(buffer, N, MPI_INT, // data to be put
	      0, // window of this rank
	      128, N, MPI_INT, // where the data to be put
	      window);
      printf("\nrank %d: unlocked!\n", rank);
      MPI_Win_unlock(0, window);
      //MPI_Barrier(MPI_COMM_WORLD);
    }
    else {
      //MPI_Barrier(MPI_COMM_WORLD);
      MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, window);
      printf("rank %d: get the lock\n", rank);
      MPI_Get(buffer, N, MPI_INT, // to store the data get
	      0, // window of this rank
	      128 + (rank - 2), N, MPI_INT, // data to be get
	      window);
      printf("rank %d: the data get is as\n", rank);
      for (i = 0;i < N;i ++)
	printf("%d\t", buffer[i]);
      printf("\nrank %d: unlocked!\n", rank);
      MPI_Win_unlock(0, window);
    }
  }
  //MPI_Barrier(MPI_COMM_WORLD);
  MPI_Win_free(&window);
  free(buffer);
  MPI_Errhandler_free(&errhdl);
  MPI_Finalize();
  return 0;
}

