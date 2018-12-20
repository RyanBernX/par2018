#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

void err_handler(MPI_Comm * comm, int * err_code, ...)
{
  int resultlen;
  char err_string[MPI_MAX_ERROR_STRING];
  MPI_Error_string(*err_code, err_string, &resultlen);
  printf("MPI exception caughted: error code %d\n", *err_code);
  printf("\terror: %s\n", err_string);
  MPI_Abort(*comm, *err_code);
}

int main(int argc, char * argv[])
{
  int i, rank, n_rank;
  int buffer;
  MPI_File fh;
  MPI_Datatype file_type;
  MPI_Datatype types[3] = {MPI_LB, MPI_INT, MPI_UB};
  MPI_Aint displacements[3];
  int counts[3] = {1, 1, 1};
  MPI_Status status;
  MPI_Errhandler errhdl;
  MPI_Init(&argc, &argv);
  MPI_Errhandler_create(&err_handler, &errhdl);
  MPI_Errhandler_set(MPI_COMM_WORLD, errhdl);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
  MPI_File_open(MPI_COMM_WORLD, "tmp.dat",
		MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, 
		&fh);
  MPI_File_preallocate(fh, n_rank*1024);
  displacements[0] = 0;
  displacements[1] = rank*sizeof(int);
  displacements[2] = n_rank*sizeof(int);
  MPI_Type_struct(3, counts, displacements, types, &file_type);
  MPI_Type_commit(&file_type);
  MPI_File_set_view(fh, 0, MPI_INT, file_type, 
		    "native", MPI_INFO_NULL);
  for (i = 0;i < 1024/sizeof(int);i ++) {
    buffer = rank*1024 + i;
    MPI_File_write_at(fh, i, &buffer, 1, MPI_INT, &status);
  }
  MPI_File_close(&fh);
  MPI_Errhandler_free(&errhdl);
  MPI_Finalize();
  return 0;
}

