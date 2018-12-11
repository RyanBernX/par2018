#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <cblas.h>

void my_multiply(int, int, int, const double *, const double *, double *);

int main(int argc, char **argv){
  int N;
  double *A, *B, *C, *C1, t, res;

  if (argc != 2){
    fprintf(stderr, "Usage: ./a.out <dim>\n");
    return 1;
  }

  N = strtol(argv[1], NULL, 10);

  A = (double*)calloc(N * N, sizeof(double));
  B = (double*)calloc(N * N, sizeof(double));
  C = (double*)calloc(N * N, sizeof(double));
  C1 = (double*)calloc(N * N, sizeof(double));

  for (int i = 0; i < N * N; ++i){
    A[i] = 2.0 * i;
    B[i] = (2.0 * i - 1);
  }

  t = omp_get_wtime();
  cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, \
    A, N, B, N, 0.0, C, N);
  printf("BLAS: %f\n", omp_get_wtime() - t);

  t = omp_get_wtime();
  my_multiply(N, N, N, A, B, C1);
  printf("My BLAS: %f\n", omp_get_wtime() - t);

  /* compare results */
  for (int i = 0; i < N * N; ++i){
    double r = fabs(C1[i] - C[i]);
    if (i == 0 || r > res)
      res = r;
  }
  printf("Error: %e\n", res);

  free(A); free(B); free(C); free(C1);
  return 0;
}

void my_multiply(int m, int n, int k, const double *A, const double *B, double *C){
  /* A: m x k    B: k x n    C: m x n */
  memset(C, 0, m * n * sizeof(double));
  
  for (int i = 0; i < m; ++i){
    for (int j = 0; j < n; ++j){
      for (int l = 0; l < k; ++l){
        C[j * m + i] += A[l * m + i] * B[j * k + l];
      }
    }
  }
}
