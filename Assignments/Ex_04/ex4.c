#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>


int main(int argc, char* argv[]){

  int rank = 0;
  int npes = 1;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  MPI_Request request;
  int n = 10;
  double start, end, w_time;
  start= MPI_Wtime();

  /* function to print the matrix */
  void print_mat(int * mat, int n_loc){
    int j,k;
    for(j=0; j<n_loc ; j++){
      for(k=0; k<n; k++){
         printf("%d ", mat[j*n + k]);
      }
      printf("\n");
    }
  }

  /* function to swap pointers */
  void swap(int ** p1, int ** p2){
    int * tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
  }



  /* shared array that stores the number rows assigned to each process */

  int * n_loc_arr = (int *)malloc(sizeof(int)*npes);
  int rest;
  int i;
  for(i=0; i<npes; i++){
    n_loc_arr[i] = n/npes;
    rest = n%npes;
    if(i<rest) n_loc_arr[i] +=1 ;
  }

  int n_loc = n_loc_arr[rank];

  /* allocation of the matrix of size n_loc*n */

  int * M = (int *)malloc(n_loc * n * sizeof(int));

  /*computation of the number of rows assigned to the processes with smaller rank*/\

  int sum_rows=0;
  for(i=0; i<rank; i++){
    sum_rows += n_loc_arr[i];
  }

  /*matrix initialization*/
  for(i = 0; i < n * n_loc; i++){
    M[i] = 0;
  }

  for(i = 0; i<n_loc; i++){
    M[sum_rows + i + n*i] = 1;
  }

  /* overlapped I/O and communication */

  if(!rank){
    int * recv_buf = (int*)malloc(n_loc * n * sizeof(int));
    int count;
    for(count=1; count<npes; count++){
      MPI_Irecv(recv_buf, n * n_loc, MPI_INT, count, 101, MPI_COMM_WORLD, &request)\
;
      print_mat(M, n_loc_arr[count-1]);
      MPI_Wait(&request, MPI_STATUS_IGNORE);
      swap(&recv_buf, &M);
    }
    print_mat(M, n_loc_arr[npes-1]);

  }
  else {
    MPI_Send(M, n*n_loc, MPI_INT, 0, 101, MPI_COMM_WORLD);
  }


  /* timing */

  end = MPI_Wtime();
  w_time = end-start;
  if(!rank) printf("time required: %lf \n", w_time);

  free(M);

  MPI_Finalize();

}




