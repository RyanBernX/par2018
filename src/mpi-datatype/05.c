#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char * argv[])
{
  int i, rank;

  int int_buffer[] = {0, 1, 2, 3};
  char char_buffer[] = "this is a test";
  double double_buffer[] = {0.1, 0.2, 0.3};

  int position, size;
  void * in_buffer;
  void * out_buffer;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  sleep(30);
  if (rank == 0) {
    size = 0;
    MPI_Pack_size(4, MPI_INT, MPI_COMM_WORLD, &i);
    size += i;
    MPI_Pack_size(14, MPI_CHAR, MPI_COMM_WORLD, &i);
    size += i;
    MPI_Pack_size(3, MPI_DOUBLE, MPI_COMM_WORLD, &i);
    size += i;
    printf("rank %d: packed data size %d\n", rank, size);
    out_buffer = (void *)malloc(size);
    position = 0;
    MPI_Pack(int_buffer, 4, MPI_INT, out_buffer, size, &position, MPI_COMM_WORLD);
    MPI_Pack(char_buffer, 14, MPI_CHAR, out_buffer, size, &position, MPI_COMM_WORLD);
    MPI_Pack(double_buffer, 3, MPI_DOUBLE, out_buffer, size, &position, MPI_COMM_WORLD);
    MPI_Send(out_buffer, size, MPI_PACKED, 1, 99, MPI_COMM_WORLD);
    free(out_buffer);
  }
  else {
    memset(int_buffer, 0, sizeof(int)*4);
    memset(char_buffer, 0, sizeof(char)*14);
    memset(double_buffer, 0, sizeof(double)*3);
    MPI_Probe(0, 99, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_PACKED, &size);
    printf("rank %d: packed data size %d probed\n", rank, size);
    in_buffer = (void *)malloc(size);
    MPI_Recv(in_buffer, size, MPI_PACKED, 0, 99, MPI_COMM_WORLD, &status);
    position = 0;
    MPI_Unpack(in_buffer, size, &position, int_buffer, 4, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(in_buffer, size, &position, char_buffer, 14, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(in_buffer, size, &position, double_buffer, 3, MPI_DOUBLE, MPI_COMM_WORLD);
    free(in_buffer);
    printf("data received as following:\n\tint buffer = {");
    for (i = 0;i < 4;i ++)
      printf(" %d ", int_buffer[i]);
    printf("}\n\tchar buffer = \"%s\"\n\tdouble buffer = {", char_buffer);
    for (i = 0;i < 3;i ++)
      printf(" %f ", double_buffer[i]);
    printf("}\n");
  }
  MPI_Finalize();
  return 0;
}

