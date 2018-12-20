/*========================================================================= 
 *
 *  Function:            double_buffer 
 *  
 *  Synopsis: 
 *       void double_buffer( 
 *               MPI_File fh,                            
 *               MPI_Datatype buftype,                  
 *               int bufcount                         
 *       ) 
 *  
 *  Description: 
 *       Performs the steps to overlap computation with a collective write 
 *       by using a double-buffering technique. 
 *  
 *  Parameters: 
 *       fh                 previously opened MPI file handle 
 *       buftype            MPI datatype for memory layout 
 *                          (Assumes a compatible view has been set on fh) 
 *       bufcount           # buftype elements to transfer 
 * ------------------------------------------------------------------------*/ 

/* this macro switches which buffer "x" is pointing to */ 
#define TOGGLE_PTR(x) (((x)==(buffer1)) ? (x=buffer2) : (x=buffer1))

void double_buffer( MPI_File fh, MPI_Datatype buftype, int bufcount) 
{

  MPI_Status status;           /* status for MPI calls */ 
  float *buffer1, *buffer2;    /* buffers to hold results */ 
  float *compute_buf_ptr;      /* destination buffer */ 
                               /* for computing */ 
  float *write_buf_ptr;        /* source for writing */ 
  int done;                    /* determines when to quit */

  /* buffer initialization */ 
  buffer1 = (float *) malloc(bufcount*sizeof(float)) ; 
  buffer2 = (float *) malloc(bufcount*sizeof(float)) ; 
  compute_buf_ptr = buffer1 ;  /* initially point to buffer1 */ 
  write_buf_ptr = buffer1 ;    /* initially point to buffer1 */

  /* DOUBLE-BUFFER prolog: 
   * compute buffer1; then initiate writing buffer1 to disk 
   */ 
  compute_buffer(compute_buf_ptr, bufcount, &done); 
  MPI_File_write_all_begin(fh, write_buf_ptr, bufcount, buftype);

  /* DOUBLE-BUFFER steady state: 
   * Overlap writing old results from buffer pointed to by write_buf_ptr 
   * with computing new results into buffer pointed to by compute_buf_ptr. 
   * 
   * There is always one write-buffer and one compute-buffer in use 
   * during steady state. */ 
  while (!done) { 
    TOGGLE_PTR(compute_buf_ptr); 
    compute_buffer(compute_buf_ptr, bufcount, &done); 
    MPI_File_write_all_end(fh, write_buf_ptr, &status); 
    TOGGLE_PTR(write_buf_ptr); 
    MPI_File_write_all_begin(fh, write_buf_ptr, bufcount, buftype); 
  }

  /* DOUBLE-BUFFER epilog: 
   * wait for final write to complete. 
   */ 
  MPI_File_write_all_end(fh, write_buf_ptr, &status);

  /* buffer cleanup */ 
  free(buffer1); free(buffer2); 
} 
