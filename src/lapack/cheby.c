/*
 * ==========================================================================
 *
 *       Filename:  cheby.c
 *
 *    Description:  Estimate all eigenvalues of an PSD matrix A larger than 1.2
 *
 *        Version:  1.0
 *        Created:  12/12/2018 07:59:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haoyang Liu (@liuhy), liuhaoyang@pku.edu.cn
 *   Organization:  BICMR, PKU
 *
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <cblas.h>
#include <lapacke.h>

#define DEGREE 15

void cheby_AX(int n, int k, const double *A, const double *X,
        double a, double b, int deg,
        double *Y);

int main(int argc, char **argv){
    int n, K, k;
    double *A, *X, *Y, *U, *W, *ev, *EV, *tau;
    int seed[4] = {23, 45, (unsigned)time(NULL) % 4096, 1};
    double t1, t2;

    if (argc != 3){
        fprintf(stderr, "Usage: ./cheby N K\n");
        return 1;
    }
    n = strtol(argv[1], NULL, 10);
    K = strtol(argv[2], NULL, 10);
    k = K / 3;

    U = (double*)malloc(n * K * sizeof(double));
    X = (double*)malloc(n * k * sizeof(double));
    Y = (double*)malloc(n * k * sizeof(double));
    A = (double*)malloc(n * n * sizeof(double));
    W = (double*)malloc(k * k * sizeof(double));
    tau = (double*)malloc(n * sizeof(double));
    ev = (double*)malloc(k * sizeof(double));
    EV = (double*)malloc(n * sizeof(double));

    /* form random symmetric A */
    LAPACKE_dlarnv(3, seed, n * K, U);
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, n, n, K,
            1.0 / n, U, n, U, n, 0.0, A, n);

    t1 = clock();
    /* initialize X with random data */
    LAPACKE_dlarnv(3, seed, n * k, X);

    /* perform Chebyshev iteration */
    cheby_AX(n, k, A, X, 0.0, 1.0, DEGREE, Y);

    /* RR projection */
    LAPACKE_dgeqrf(LAPACK_COL_MAJOR, n, k, Y, n, tau);
    LAPACKE_dorgqr(LAPACK_COL_MAJOR, n, k, k, Y, n, tau);

    /* X = A * Y */
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, k, n,
            1.0, A, n, Y, n, 0.0, X, n);

    /* W = X' * Y */
    cblas_dgemm(CblasColMajor, CblasTrans, CblasNoTrans, k, k, n,
            1.0, X, n, Y, n, 0.0, W, k);
    /* compute eigenvalues of W */
    LAPACKE_dsyevd(LAPACK_COL_MAJOR, 'N', 'U', k, W, k, ev);
    t1 = (clock() - t1) / CLOCKS_PER_SEC;

    /* compute eigenvalues of A to check the results */
    t2 = clock();
    LAPACKE_dsyevd(LAPACK_COL_MAJOR, 'N', 'U', n, A, n, EV);
    t2 = (clock() - t2) / CLOCKS_PER_SEC;

    printf("    |   TRUE EV       ESTIMATED\n");
    printf("---------------------------------\n");
    for (int i = 1; i <= k; ++i){
        printf("%3d | %12.6e  %12.6e\n", i, EV[n - i], ev[k - i]);
    }
    printf("---------------------------------\n");
    printf("time|   %6.2f sec    %6.2f sec\n", t2, t1);
    free(U); free(X); free(Y); free(A);
    free(W); free(tau); free(ev); free(EV);
    return 0;
}

void cheby_AX(int n, int k, const double *A, const double *X,
        double a, double b, int deg,
        double *Y){

    double c1 = 2 / (b - a), c0 = (a + b) / (a - b);
    double *T1, *T2;
    int nk = n * k;
    /* set Y = X */
    cblas_dcopy(nk, X, 1, Y, 1);
    if(deg == 0) return;
    if(deg == 1){
        cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, k, n,
                c1, A, n, X, n, c0, Y, n);
        return;
    }

    T1 = (double*)malloc(nk * sizeof(double));
    T2 = (double*)malloc(nk * sizeof(double));

    /* T1 = X */
    cblas_dcopy(nk, X, 1, T1, 1);
    /* T2 = c0 * X + c1 * A * X */
    cblas_dcopy(nk, X, 1, T2, 1);                                            
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, k, n,
            c1, A, n, X, n, c0, T2, n);

    for(int i = 2; i <= deg; ++i ){
        /* Y = 2 * c0 * T2 + 2 * c1 * A * T2 */
        cblas_dcopy(nk, T2, 1, Y, 1);
        cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, n, k, n,
                2 * c1, A, n, T2, n, 2 * c0, Y, n);
        /* Y = Y - T1 */
        cblas_daxpy(nk, -1, T1, 1, Y, 1);                                      
        if(i < deg){
            /* T1 = T2 */
            cblas_dcopy(nk, T2, 1, T1, 1);
            /* T2 = Y */
            cblas_dcopy(nk, Y, 1, T2, 1);
        }                                                                           
    }                                                                             
    free(T1); free(T2);                                                           
}
