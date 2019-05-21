#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>



double local_sum(double local_a, double local_b, double h ){

  double local_result=0.0;
  while(local_a<=local_b){
    double x_i = local_a + h*0.5;
    local_result += 1.0/(1.0+x_i*x_i);
    local_a = local_a+h;
  }
  local_result = h * local_result;
  return local_result;
}


int main(int argc, char* argv[]){

  int rank = 0;
  int npes = 1;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  double t1;
  double t2;

  t1 = MPI_Wtime();

  double n = 1000000000;
  double h = 1.0/n;
  double local_n = n/npes;

  double local_a =  rank * local_n * h;
  double local_b = local_a + local_n * h;
  double local_res = local_sum(local_a, local_b, h);

  double global_res;

  MPI_Reduce(&local_res, &global_res, 1, MPI_DOUBLE, MPI_SUM, npes-1, MPI\
_COMM_WORLD);
  double pi;
  if(rank == npes-1){
    pi = global_res*4;
    MPI_Send(&pi,1,MPI_DOUBLE,0, 101, MPI_COMM_WORLD);
  }

  t2 = MPI_Wtime();


  if(rank==0){
    MPI_Recv(&pi,1, MPI_DOUBLE, npes-1, 101, MPI_COMM_WORLD, MPI_STATUS_I\
GNORE);
    printf("Pi approximation: %lf \n", pi);
    printf("Elapsed time: %lf \n", t2-t1);
  }

  MPI_Finalize();


  return 0;

}




