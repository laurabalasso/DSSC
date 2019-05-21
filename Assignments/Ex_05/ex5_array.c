#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void swap(int ** a, int ** b ){
  int * tmp = *a;
  *a = *b;
  *b = tmp;
}


int main(int argc, char* argv[]){

  int rank = 0;
  int npes = 1;
  int i;
  int j;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  /* allocate the buffer for communication and the array to store the sum */
  int* send_buf = (int *)malloc(sizeof(int)*npes);
  int* recv_buf = (int *)malloc(sizeof(int)*npes);
  int* sum = (int *)malloc(sizeof(int)*npes);


  for(i = 0; i<npes; i++){
    send_buf[i] = rank;
    sum[i] = 0;
  }

  MPI_Request request;

  for(i = 0; i < npes; i++ ){

    MPI_Isend(send_buf, npes, MPI_INT, (rank+1)%npes , 101, MPI_COMM_WORLD, &request);
    for(j=0; j<npes; j++){
      sum[j] += send_buf[j];
    }

    MPI_Irecv(recv_buf, npes, MPI_INT, (rank-1+npes)%npes, 101, MPI_COMM_WORLD, &request);

    MPI_Wait(&request, MPI_STATUS_IGNORE);
    swap(&send_buf, &recv_buf);
  }



  printf("process %d sum:\n", rank);
  for(i=0; i<npes; i++){
    printf("%d  ", sum[i]);
  }
  printf("\n");

  MPI_Finalize();

  free(sum);
  free(send_buf);
  free(recv_buf);
  return 0;

}




