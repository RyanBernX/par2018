#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <mpi.h>

int is_end = 0;
int rank;

/**
 * MPI�������� 
 * ������������򵥵Ĵ�ӡ������ź���Ӧ��˵���ַ�����Ȼ���˳�MPIϵͳ
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
 * POSIX�źŴ����� 
 * ��is_end��Ϊ1������ӡ���źŲ�׽�������Ϣ
 */
void signal_handler(int sig)
{
  is_end = 1;
  printf("rank %d: pid %d: signal TERM caught, set is_end as %d to terminate ...\n", 
	 rank, getpid(), is_end);
}

/**
 * ���߳� 
 * ���߳�ÿ����������1���̷���һ��is_end����is_end����1ʱ�˳���������ٷ���һ��is_end,
 * ��֪ͨ��1���̽���������ӡ��������Ϣ
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
  if (rank == 0) { // ��0����
    // �������߳�
    ret = pthread_create(&thread, NULL, &child_function, NULL);
    if (ret != 0) { // ����������߳�ʧ�ܣ����˳�
      printf("create thread failure, error code: %d\n", ret);
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    else { // �������ø��̵߳��źŴ�����
      signal(SIGTERM, &signal_handler);
      printf("send signal TERM to process %d to terminate.\n", getpid());
      pthread_join(thread, NULL); // �ȴ����̷߳���
    }
  }
  else { // ��1����
    while (!is_end) { // ÿ5���ӽ���һ��is_end
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

