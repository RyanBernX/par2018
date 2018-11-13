/*
 * ===========================================================================
 *
 *       Filename:  001.c
 *
 *    Description:  test001
 *
 *        Version:  1.0
 *        Created:  11/13/2018 08:12:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haoyang Liu (), liuhaoyang@pku.edu.cn
 *   Organization:  BICMR, Peking University
 *      Copyright:  Copyright (c) 2018, Haoyang Liu
 *
 * ===========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char * argv[]){
    int rank, size, buffer;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    sleep(60);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0){
        buffer = 7;
        MPI_Send(&buffer, 1, MPI_INT, 1, 99, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&buffer, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
        printf("Process %d:\"Got a %d\"\n", rank, buffer);
    }

    MPI_Finalize();
    return 0;
}
