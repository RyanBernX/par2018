#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <omp.h>


int main(int argc, char **argv){
  int rank, size;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

# pragma omp parallel
  {
    int omp_rank = omp_get_thread_num();
    int omp_size = omp_get_num_threads();
    char hostname[100];
    gethostname(hostname, 100);
    if ( omp_rank == 0 ){
      printf("This is process %d of %d on host %s, I have %d threads.\n", rank, size, hostname, omp_size);
    }
  }

  MPI_Finalize();
  return 0;
}

