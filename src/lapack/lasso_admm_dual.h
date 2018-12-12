/*
 * ==========================================================================
 *
 *       Filename:  lasso_admm_dual.h
 *
 *    Description:  Header file for solving lasso using ADMM on dual
 *
 *        Version:  1.0
 *        Created:  11/27/2018 04:15:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haoyang Liu (@liuhy), liuhaoyang@pku.edu.cn
 *   Organization:  BICMR, PKU
 *
 * ==========================================================================
 */

#ifndef LASSO_ADMM_DUAL_H
#define LASSO_ADMM_DUAL_H

typedef struct{
    double ftol;
    int maxit;
    int verbose;
    double sigma;
    double threshold;
    int do_threshold;
} admm_option;

int lasso_admm(int m, int n, const double *A, const double *b, double mu,
        double *x, double *y,
        admm_option option);

void projection_onto_linf_ball(
        int n,
        double *x,
        double r);

double lasso_admm_dual_f(
        int n,
        const double *b,
        const double *y);

double lasso_admm_dual_infeasi(
        int n,
        const double *Aty,
        double       mu);

#endif

