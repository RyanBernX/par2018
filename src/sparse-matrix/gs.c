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
        double        *b){

    for (int i = 0; i < A.m; ++i){
        double ri = b[i];
        for (int j = A.rs[i] + 1; j < A.rs[i + i]; ++j){
            ri -= A.ev[j] * x[A.ci[j]];
        }
        x[i] = ri / A.ev[A.rs[i]];
    }

    while (residual > tolerance);
}

/*
 * A.n = ???
 * A.m = ???
 *
 * A.rs = (int*) malloc( (A.m + 1) * sizeof(int));
 * A.rs[0] = 0;
 *
 * K
 * A.rs[i] = i * K; for all i
 *
 * ci = (int*)malloc( (K * m) * sizeof(int));
 * ci[j] = n;
 *
 * {
 *     add_nz_entry();
 *     ...
 *     add_nz_entry();
 * }
 *
 * A.ci = malloc();
 *
 * void add_nz_entry(int *rs, int *ci, int i, int j){
 *     for (k = rs[i]; k < rs[i + 1]; ++k){
 *         if (ci[k] == n || ci[k] == j){
 *             ci[k] = j;
 *             break;
 *         }
 *     }
 * }
 */

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

