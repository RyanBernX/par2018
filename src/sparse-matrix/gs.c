/*
 * ===========================================================================
 *
 *       Filename:  gs.c
 *
 *    Description:  Sample codes for Gauss-Seidel iteration
 *
 *        Version:  1.0
 *        Created:  12/06/2018 11:19:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haoyang Liu (), liuhaoyang@pku.edu.cn
 *   Organization:  BICMR, Peking University
 *      Copyright:  Copyright (c) 2018, Haoyang Liu
 *
 * ===========================================================================
 */
#include <stdlib.h>
typedef struct _sparse_matrix{
    int m, n; // number of rows, columns

    int *rs;  // row start index
    int *ci;  // column index

    double *ev;
} sparse_matrix;

void gs(sparse_matrix  A,
        double        *x,
        double        *b,
        double       tol){

    do {
        for (int i = 0; i < A.m; ++i){
            double ri = b[i];
            for (int j = A.rs[i] + 1; j < A.rs[i + i]; ++j){
                ri -= A.ev[j] * x[A.ci[j]];
            }
            x[i] = ri / A.ev[A.rs[i]];
        }

    } while (res > tol); // residual smaller than tolerence
}

void init_pattern(sparse_matrix *A,
                  int m,
                  int n,
                  int K){
                  
    A->n = n;
    A->m = m;

    A->rs = (int*) malloc((m + 1) * sizeof(int));
    A->rs[0] = 0;

    for (int i = 0; i <= m; ++i){
        A->rs[i] = i * K;
    }

    A->ci = (int*) malloc((K * m) * sizeof(int));
    for (int i = 0; i < K * m; ++i){
        A->ci[i] = n;
    }
}


void compress(sparse_matrix *A){
    int nnz = 0;
    int * ci = A->ci;
    for (int i = 0; i < A->m; ++i){
        for (int j = A->rs[i]; j < A->rs[i + 1]; ++j){
            if (ci[j] == n) {
                int nnz_old = nnz;
                nnz += j - A->rs[i];
                A->rs[i] = nnz_old;
                break;
            }
        }
    }
    A->rs[m] = nnz;
    A->ci = (int *)malloc(nnz * sizeof(int));
    for (int i = 0; i < A->m; ++i){
        for (int j = A->rs[i]; j < A->rs[i + 1]; ++j){
            A->ci[j] = ci[i * K + j - A->rs[i]];
        }
    }

    free(ci);
    A->ev = (double *)malloc(nnz * sizeof(double));
}


void add_nz_entry(sparse_matrix A, int i, int j){
    int k;
    for (k = A.rs[i]; k < A.rs[i + 1]; ++k){
        if (A.ci[k] == A.n || A.ci[k] == j){
            A.ci[k] = j;
            break;
        }
    }
}
void add_nz_value(sparse_matrix A, int i, int j, double v){
    int k;
    for (k = A.rs[i]; k < A.rs[i + 1]; ++k){
        if (A.ci[k] == j){
            A.ev[k] = v;
            break;
        }
    }
}


void mv(sparse_matrix  A,
        double        *x,
        double        *b){
    
    for (int i = 0; i < A.m; ++i){
        b[i] = 0;
        for (int j = A.rs[i]; j < A.rs[i + i]; ++j){
            b[i] += A.ev[j] * x[A.ci[j]];
        }
    }
}

void mTv(sparse_matrix  A,
         double        *x,
         double        *b){
    
    for (int i = 0; i < A.m; ++i){
        b[i] = 0;
        for (int j = A.rs[i]; j < A.rs[i + i]; ++j){
            b[A.ci[j]] += A.ev[j] * x[i];
        }
    }
}

void solve_possion(int N){
    sparse_matrix A;
    init_pattern(&A, N * N, N * N, 5);

    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            int k = i * N + j;
            add_nz_entry(A, k, k);
            if (i > 0)     add_nz_entry(A, k, k - 1);
            if (i < N - 1) add_nz_entry(A, k, k + 1);
            if (j > 0)     add_nz_entry(A, k, k - N);
            if (j < N - 1) add_nz_entry(A, k, k + N);
        }
    }

    compress(&A);

    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j){
            int k = i * N + j;
            add_nz_value(A, k, k, 4);
            if (i > 0)     add_nz_value(A, k, k - 1, -1);
            if (i < N - 1) add_nz_value(A, k, k + 1, -1);
            if (j > 0)     add_nz_value(A, k, k - N, -1);
            if (j < N - 1) add_nz_value(A, k, k + N, -1);
        }
    }

    // b, x
    gs(A, x, b, tol);
}

void mm(sparse_matrix A,
        sparse_matrix B,
        sparse_matrix *C){

    int K;
    int index[B.n];
    int flag[B.n] = {0};
    /* attempt to multiply A and B (determine max_nnz) */
    for (int i = 0; i < A.m; ++i){
        int nz = 0; /* nnz of i-th row */
        for (int j = A.rs[i]; j < A.rs[i + 1]; ++j){
            int i1 = A.ci[j];
            for (int j1 = B.rs[i1]; j1 < B.rs[i1 + 1]; ++j1){
                int k = B.ci[j1];
                if (flag[k] == 0){
                    index[nz++] = k;
                    flag[k] = 1;
                }
            }
        }
    }
    K = max(K, nz);
    for (int j = 0; j < nz; ++j){
        flag[j] = 0;
    }

    init_pattern(C, A.m, B.n, K);
}
