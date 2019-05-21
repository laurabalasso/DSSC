#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void swap(int * a, int * b ){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


int main(int argc, char* argv[]){
    
    int rank = 0;
    int npes = 1;
    int i;
    
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &npes );
    
    /* allocate the buffer for communication and the array to store the sum */
    int send_buf = rank;
    int recv_buf;
    int sum = 0;
    
      
    MPI_Request request;
    
    for(i = 0; i < npes; i++ ){
        
        MPI_Isend(&send_buf, 1, MPI_INT, (rank+1)%npes , 101, MPI_COMM_WORLD, &request);

        sum += send_buf;
        
        
        MPI_Irecv(&recv_buf, 1, MPI_INT, (rank-1+npes)%npes, 101, MPI_COMM_WORLD, &request);
        
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        swap(&send_buf, &recv_buf);
    }
    
    
    
    printf("process %d sum:\n", rank);
    printf("%d \n ", sum);
    
    
    MPI_Finalize();
    
    return 0;
    
}
