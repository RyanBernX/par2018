/*
 * ==========================================================================
 *
 *       Filename:  lasso_admm_dual.c
 *
 *    Description:  Source file for solving lasso using ADMM dual
 *
 *        Version:  1.0
 *        Created:  11/27/2018 04:31:29 PM
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
#include <string.h>
#include <math.h>
#include <time.h>
#include <cblas.h>
#include <lapacke.h>
#include "lasso_admm_dual.h"

#define SPARSITY 0.1

int main(int argc, char **argv){
    int m, n;
    double *A, *b, *u, *x, *y;
    int ipos, seed[4] = {23, 45, (unsigned)time(NULL) % 4096, 1};

    if (argc != 3){
        fprintf(stderr, "Usage: ./admm M N\n");
        return 1;
    }
    m = strtol(argv[1], NULL, 10);
    n = strtol(argv[2], NULL, 10);

    A = (double *)malloc(m * n * sizeof(double));
    b = (double *)malloc(m * sizeof(double));
    u = (double *)calloc(n, sizeof(double));
    x = (double *)calloc(n, sizeof(double));
    y = (double *)calloc(m, sizeof(double));

    /* Gaussian data */
    LAPACKE_dlarnv(3, seed, m * n, A);
    for (int i = 0; i < n * SPARSITY; ++i){
        double r;
        LAPACKE_dlarnv(1, seed, 1, &r);
        ipos = floor(r * n);
        LAPACKE_dlarnv(3, seed, 1, u + ipos);
    }
    cblas_dgemv(CblasColMajor, CblasNoTrans, m, n, 1.0, A, m, u, 1, 0, b, 1);

    /* prepare option */
    admm_option option;
    option.maxit = 1000;
    option.ftol = 1e-9;
    option.sigma = 5;
    option.verbose = 1;
    option.threshold = 1e-5;
    option.do_threshold = 1;

    /* call ADMM */
    LAPACKE_dlarnv(3, seed, n, x);
    cblas_dcopy(m, b, 1, y, 1);
    cblas_dgemv(CblasColMajor, CblasNoTrans, m, n, -1.0, A, m, x, 1, 0, y, 1);
    lasso_admm(m, n, A, b, 1e-3, x, y, option);

    printf("\n\n");
    printf("    |  TRUTH        LASSO\n");
    printf("---------------------------\n");
    for (int i = 0; i < n; ++i){
        /* only print non-zero entries */
        if (fabs(u[i]) > 1e-14 || fabs(x[i]) > 1e-14){
            printf("%3d | %10.4e  %10.4e\n", i, u[i], x[i]);
        }
    }

    free(A); free(b); free(u); free(x); free(y);

    return 0;
}

int lasso_admm(int m, int n, const double *A, const double *b, double mu,
        double *x, double *y, admm_option option){

    /* initialization */
    double *z, *tmp, *Aty;
    double *AAt;
    double f, fp, Fdiff, feasi;

    AAt = (double*)calloc(m * m, sizeof(double));
    z   = (double*)malloc(n * sizeof(double));
    tmp = (double*)malloc(n * sizeof(double));
    Aty = (double*)malloc(n * sizeof(double));

    /* pre-computing chol */
    for (int i = 0; i < m; ++i){
        AAt[i * (m + 1)] = 1;
    }
    cblas_dsyrk(CblasColMajor, CblasUpper, CblasNoTrans, m, n, option.sigma,
            A, m, 1.0, AAt, m);
    LAPACKE_dpotrf(LAPACK_COL_MAJOR, 'U', m, AAt, m);

    f = lasso_admm_dual_f(m, b, y);
    /* compute Aty */
    cblas_dgemv(CblasColMajor, CblasTrans, m, n, 1.0, A, m, y, 1, 0, Aty, 1);

    /* main loop */
    for (int iter = 0; iter < option.maxit; ++iter){
        fp = f;
        /* update z */
        cblas_dcopy(n, Aty, 1, z, 1);
        cblas_daxpy(n, 1.0 / option.sigma, x, 1, z, 1);
        projection_onto_linf_ball(n, z, mu);

        /* update y */
        cblas_dcopy(n, x, 1, tmp, 1);
        cblas_dscal(n, -1, tmp, 1);
        cblas_daxpy(n, option.sigma, z, 1, tmp, 1);

        cblas_dcopy(m, b, 1, y, 1);
        cblas_dgemv(CblasColMajor, CblasNoTrans, m, n, 1.0, A, m, tmp, 1, 1.0, y, 1);
        LAPACKE_dpotrs(LAPACK_COL_MAJOR, 'U', m, 1, AAt, m, y, m);

        /* update x (multiplier) */
        cblas_dgemv(CblasColMajor, CblasTrans, m, n, 1.0, A, m, y, 1, 0, Aty, 1);
        cblas_dcopy(n, z, 1, tmp, 1);
        cblas_daxpy(n, -1, Aty, 1, tmp, 1);
        cblas_daxpy(n, -option.sigma, tmp, 1, x, 1);

        /* check stopping rule */
        f = lasso_admm_dual_f(m, b, y);
        Fdiff = fabs(fp - f) / fabs(f);
        feasi = lasso_admm_dual_infeasi(n, Aty, mu);

        if (option.verbose){
            if ( iter == 0 ){
                printf("  itr     dfval       Fdiff       dfeas\n");
                printf("-----------------------------------------\n");
            }

            if ( iter % 10 == 0){
                printf("%5d  %10.3e  %10.3e  %10.3e\n", iter, f, Fdiff, feasi);
            }
        }

        /* optional thresholding */
        if (option.do_threshold){
            for (int i = 0; i < n; ++i){
                if (fabs(x[i]) < option.threshold) x[i] = 0;
            }
        }

        if ( Fdiff < option.ftol && feasi < 1E+4 * option.ftol){
            break;
        }

    }

    free(AAt); free(tmp); free(z); free(Aty);
    return 0;
}

void projection_onto_linf_ball(
        int n,
        double *x,
        double  r){

    for (int i = 0; i < n; ++i){
        if (x[i] >  r) x[i] =  r;
        if (x[i] < -r) x[i] = -r;
    }
}


double lasso_admm_dual_f(
        int m,
        const double *b,
        const double *y){

    double norm_y = cblas_ddot(m, y, 1, y, 1);
    double ytb = cblas_ddot(m, y, 1, b, 1);

    return 0.5 * norm_y - ytb;
}

double lasso_admm_dual_infeasi(
        int n,
        const double *Aty,
        double  mu){

    double feasi, norm;
    int imax;

    imax = cblas_idamax(n, Aty, 1);
    norm = fabs(Aty[imax]);
    feasi = norm - mu > 0 ? norm - mu : 0;
    feasi /= mu;

    return feasi;

}


