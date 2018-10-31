#!/bin/bash
#SBATCH -J MPI_test
#SBATCH -p compute
#SBATCH -t 5:00
#SBATCH -N 2
#SBATCH --ntasks-per-node=1
#SBATCH -c 2
#SBATCH -o MPI_test.log


#module add mpi/mpich/3.2.1
module add mpi/openmpi/3.1.2

export OMP_NUM_THREADS=2
mpiexec -n 2 ./test
mpiexec -n 2 ./send

