#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double x;
int rank;

void operator(void * invec, void * inoutvec,
	      int * length, MPI_Datatype * data_type)
{
  int i;
  double b0;
  double * a = (double *)invec;
  double * b = (double *)inoutvec;
  printf("rank %d: {", rank);
  for (i = 0;i < *length;i ++) {
    b0 = b[i];
    b[i] = a[i]*x + b[i];
    printf("[%f,%f;%f], ", a[i], b0, b[i]); 
  }
  printf("}\n");
}


int main(int argc, char * argv[])
{
  int i, n_rank;
  MPI_Op op;
  double buffer[4], rbuffer[4];
  MPI_Init(&argc, &argv);
  x = atof(argv[1]);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
  MPI_Op_create(&operator, 0, &op);
  for (i = 0;i < 4;i ++) {
    buffer[i] = rank*i*i + rank*(i + 1) + 1;
  }
  printf("rank %d: a={%f, %f, %f, %f}\n", rank, 
         buffer[0], buffer[1], buffer[2], buffer[3]);
  MPI_Reduce(buffer, rbuffer, 4, MPI_DOUBLE, op, 0, MPI_COMM_WORLD);
  MPI_Op_free(&op);
  MPI_Finalize();
  if (rank == 0)
    printf("rank %d: {%f, %f, %f, %f}\n", rank, 
           rbuffer[0], rbuffer[1], rbuffer[2], rbuffer[3]);
  return 0;
}

