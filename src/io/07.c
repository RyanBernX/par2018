#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <mpi.h>

int is_end = 0;
int rank;

/**
 * MPI错误处理器 
 * 这个错误处理器简单的打印处错误号和相应的说明字符串，然后退出MPI系统
 */
void err_handler(MPI_Comm * comm, int * err_code, ...)
{
  int resultlen;
  char err_string[MPI_MAX_ERROR_STRING];
  MPI_Error_string(*err_code, err_string, &resultlen);
  printf("MPI exception caughted: error code %d\n", *err_code);
  printf("\terror: %s\n", err_string);
  MPI_Abort(*comm, *err_code);
}

/**
 * POSIX信号处理器 
 * 将is_end设为1，并打印出信号捕捉情况的信息
 */
void signal_handler(int sig)
{
  is_end = 1;
  printf("rank %d: pid %d: signal TERM caught, set is_end as %d to terminate ...\n", 
	 rank, getpid(), is_end);
}

/**
 * 子线程 
 * 子线程每两秒钟向秩1进程发送一次is_end，在is_end等于1时退出，并最后再发送一次is_end,
 * 以通知秩1进程结束，最后打印出结束信息
 */
void * child_function(void * args)
{
  printf("rank %d: child thread created with pid %d\n", rank, getpid());
  sleep(2);
  while (!is_end) {
    printf("rank %d: is_end = %d, sleeping ...\n", rank, is_end);
    sleep(2);
    MPI_Send(&is_end, 1, MPI_INT, 1, 99, MPI_COMM_WORLD);
  }
  MPI_Send(&is_end, 1, MPI_INT, 1, 99, MPI_COMM_WORLD);
  printf("rank %d: is_end = %d, ending ...\n", rank, is_end);
  return NULL;
}

int main(int argc, char * argv[])
{
  int ret;
  pthread_t thread;
  MPI_Status status;
  MPI_Errhandler errhdl;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Errhandler_create(&err_handler, &errhdl);
  MPI_Errhandler_set(MPI_COMM_WORLD, errhdl);
  if (rank == 0) { // 秩0进程
    // 创建子线程
    ret = pthread_create(&thread, NULL, &child_function, NULL);
    if (ret != 0) { // 如果创建子线程失败，则退出
      printf("create thread failure, error code: %d\n", ret);
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    else { // 否则设置父线程的信号处理句柄
      signal(SIGTERM, &signal_handler);
      printf("send signal TERM to process %d to terminate.\n", getpid());
      pthread_join(thread, NULL); // 等待子线程返回
    }
  }
  else { // 秩1进程
    while (!is_end) { // 每5秒钟接收一次is_end
      printf("rank %d: is_end = %d, sleeping ...\n", rank, is_end);
      sleep(5);
      MPI_Recv(&is_end, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
    }
    printf("rank %d: is_end = %d, ending ...\n", rank, is_end);
  }
  MPI_Errhandler_free(&errhdl);
  MPI_Finalize();
  return 0;
}

