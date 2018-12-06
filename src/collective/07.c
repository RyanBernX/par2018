#include <stdio.h>
#include <mpi.h>

int rank;

void operator(void * invec, void * inoutvec,
	      int * length, MPI_Datatype * data_type)
{
  int i;
  int * a = (int *)invec;
  int * b = (int *)inoutvec;
  printf("rank %d:", rank);
  for (i = 0;i < *length;i ++) {
    printf("{a[%d] = %d, b[%d] = (%d,", i, a[i], i, b[i]);
    b[i] = b[i] + a[i]*a[i];
    b[i] %= 473;
    printf("%d)}", b[i]);
  }
  printf("\n\n");
}


int main(int argc, char * argv[])
{
  int i;
  MPI_Op op;
  int buffer[3] = {1, 7, 19}, obuffer[3];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  for (i = 0;i < 3;++ i) {
    buffer[i] += rank;
  }
  printf("rank %d: {%d, %d, %d}\n", rank, buffer[0], buffer[1],
	 buffer[2]);
  MPI_Op_create(&operator, 0, &op);
  MPI_Scan(buffer, obuffer, 3, MPI_INT, op, MPI_COMM_WORLD);
  MPI_Op_free(&op);
  MPI_Finalize();
  printf("rank %d: {%d, %d, %d}\n", rank, obuffer[0], obuffer[1],
	 obuffer[2]);
  return 0;
}

