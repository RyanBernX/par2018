// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Example using MPI_Send and MPI_Recv to pass a message around in a ring.
//
// Modified by Haoyang Liu to prevent deadlock
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1000000

int main(int argc, char** argv) {
  int rank;
  int world_size;
  int *buf1, *buf2;
  int dst, src;
  MPI_Status status;

  // Initialize the MPI environment
  MPI_Init(&argc, &argv);
  // Find out rank, size
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  // Initialize data
  buf1 = (int*)malloc(SIZE * sizeof(int));
  buf2 = (int*)malloc(SIZE * sizeof(int));
  if (buf1 == NULL || buf2 == NULL){
    fprintf(stderr, "Memory allocation failed. Aborting.\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
    return 1;
  }
  memset(buf1, rank + 1, SIZE * sizeof(int));

  dst = (rank == world_size-1) ? 0 : rank + 1;
  src = (rank == 0) ? world_size - 1 : rank - 1;

  // avoid deadlock
  if (rank % 2 == 0){
    MPI_Send(buf1, SIZE, MPI_INT, dst, 0, MPI_COMM_WORLD);
    printf("process %d: sent %d integers to %d with %d\n", rank, SIZE, dst, buf1[0]);
  }

  MPI_Recv(buf2, SIZE, MPI_INT, src, 0, MPI_COMM_WORLD, &status);  
  printf("process %d: recieved from %d: %d\n", rank, src, buf2[0]);
    
  if (rank % 2 == 1){
    MPI_Send(buf1, SIZE, MPI_INT, dst, 0, MPI_COMM_WORLD);
    printf("process %d: sent %d integers to %d with %d\n", rank, SIZE, dst, buf1[0]);
  }
  
  free(buf1);
  free(buf2);
  MPI_Finalize();
  return 0;
}
