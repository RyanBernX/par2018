#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char * argv[])
{
  int i, j, m, n;
  int * matrix;
  int * transposed_matrix;
  FILE * fp;
  MPI_Datatype matrix_type;
  MPI_Datatype column_type;
  MPI_Datatype transposed_matrix_type;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  m = atoi(argv[1]);
  n = atoi(argv[2]);
  matrix = (int *)calloc(m*n, sizeof(int));
  transposed_matrix = (int *)calloc(m*n, sizeof(int));

  fp = fopen("matrix.dat", "w");
  for (i = 0;i < m;i ++) {
    for (j = 0;j < n;j ++) {
      matrix[i*n + j] = i*n + j;
      fprintf(fp, "%d\t", matrix[i*n + j]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

  //MPI_Type_contiguous(m*n, MPI_INT, &matrix_type);
  MPI_Type_vector(m, n, n, MPI_INT, &matrix_type);
  MPI_Type_commit(&matrix_type);
  MPI_Type_vector(n, 1, m, MPI_INT, &column_type);
  MPI_Type_commit(&column_type);
  MPI_Type_hvector(m, 1, sizeof(int), column_type, &transposed_matrix_type);
  MPI_Type_commit(&transposed_matrix_type);
  MPI_Sendrecv(matrix, 1, matrix_type, 0, 99,
	       transposed_matrix, 1, transposed_matrix_type, 0, 99,
	       MPI_COMM_SELF, &status);
  MPI_Type_free(&matrix_type);
  MPI_Type_free(&column_type);
  MPI_Type_free(&transposed_matrix_type);
  MPI_Finalize();

  fp = fopen("transposed_matrix.dat", "w");
  for (i = 0;i < n;i ++) {
    for (j = 0;j < m;j ++) {
      fprintf(fp, "%d\t", transposed_matrix[i*m + j]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
  return 0;
}

