#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);


  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 100;
  int numbers[MAX_NUMBERS];
  int number_amount;
  MPI_Status status;

  // initialize random seed
  srand(time(NULL) + 1000 * world_rank);

  if (world_rank != 0) {
    // Pick a random amount of integers to send to process one
    number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

    // Send the amount of integers to process one
    MPI_Send(numbers, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD);
    printf("%d sent %d numbers to 0\n",world_rank, number_amount);

  } else {

    for(int k=0;k<world_size-1;k++) {
      // Receive at most MAX_NUMBERS from process zero
      MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

      // After receiving the message, check the status to determine how many
      // numbers were actually received
      MPI_Get_count(&status, MPI_INT, &number_amount);

      // Print off the amount of numbers, and also print additional information
      // in the status object
      printf("0 received %d numbers from source = %d, tag = %d\n",
             number_amount, status.MPI_SOURCE, status.MPI_TAG);
	}
  }
 
  MPI_Finalize();
}
