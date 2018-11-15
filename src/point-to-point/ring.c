// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// Example using MPI_Send and MPI_Recv to pass a message around in a ring.
//
#include <mpi.h>
#include <stdio.h>
#include <string.h>
#define SIZE 1000

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(&argc, &argv);
  // Find out rank, size
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  static int buf1[SIZE],buf2[SIZE];
  MPI_Status status;
  int dst;
  int src;

  memset(buf1, rank + 1, SIZE * sizeof(int));

  dst = (rank == world_size-1)? 0:rank+1;
  src = (rank==0)? world_size-1:rank-1;

  MPI_Send(buf1, SIZE, MPI_INT, dst, 0, MPI_COMM_WORLD);
  printf("process %d: sent %d integers to %d with %d\n", rank, SIZE, dst, buf1[0]);

  MPI_Recv(buf2, SIZE, MPI_INT, src, 0, MPI_COMM_WORLD, &status);  
  printf("process %d: recieved from %d: %d\n", rank, src, buf2[0]);

  MPI_Finalize();
  return 0;
}
