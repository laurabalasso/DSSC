//
//  fast_transpose.cu
//
//
//  Created by Laura Balasso on 13/05/2019.
//

#include <stdio.h>
#include <stdlib.h>

#define TILE_DIM 32

/* function that fills an array with random doubles */

void random_doubles(double *p, int n) {
    int i;
    for(i=0; i<n; i++) {
        p[i]= ( (double)rand() * 100 ) / (double)RAND_MAX ;
    }
}


/* function that tests the equality between two martices */

void equality_test(double* M1, double* M2, int N){
    long int i;
    for(i=0; i<N*N; i++){
        if(M1[i]!=M2[i]){
            printf("Error! the two methods produce different results. \n"\
);
            break;
        }
    }
    if(i == N*N) printf("Correct result! \n");
}


/* kernel that implements the fast transpose */

__global__ void fast_transpose(double * M_in , double * M_out, int block_rows){

    __shared__ double tile[TILE_DIM][TILE_DIM]; // allocate the tile in shared memory (one per block)

    int x = blockIdx.x * TILE_DIM + threadIdx.x; // define index
    int y = blockIdx.y * TILE_DIM + threadIdx.y;
    int width = gridDim.x * TILE_DIM; // compute matrix width

    /* each block of threads copies a TILE_DIM x TILE_DIM submatrix in the tile */
    /* some threads handle more than one element since THREADS_PER_BLOCK < TILE_DIM*TILE_DIM */

    for (int j = 0; j < TILE_DIM; j += block_rows){
        tile[threadIdx.y+j][threadIdx.x] = M_in[(y+j) * width + x];
    }
    __syncthreads(); // ensures that all the threads copied the values in the tile

    x = blockIdx.y * TILE_DIM + threadIdx.x;
    y = blockIdx.x * TILE_DIM + threadIdx.y;

    for (int j = 0; j < TILE_DIM; j += block_rows){
        M_out[(y+j)*width + x] = tile[threadIdx.x][threadIdx.y + j];
    }
}

/* kernel that implements a naive algorithm for matrix transpose */

__global__ void naive_transpose(double * M_in, double * M_out, int block_rows)
{
    int x = blockIdx.x * TILE_DIM + threadIdx.x;
    int y = blockIdx.y * TILE_DIM + threadIdx.y;
    int width = gridDim.x * TILE_DIM;

    for (int j = 0; j < TILE_DIM; j+= block_rows){
        M_out[x*width + (y+j)] = M_in[(y+j)*width + x];
    }
}

/* function that runs both the naive and the blocking kernels for a given number of threads per block and matrix size */

void run_kernel(int threads_per_block, int dimx, int dimy){


    /* allocate host matrices */
    int num_bytes = dimx*dimy*sizeof(double);
    double *h_in = (double*)malloc(num_bytes);
    double *h_out_block = (double*)malloc(num_bytes);
    double *h_out_naive = (double*)malloc(num_bytes);

    /* allocate davice matrices */
    double *d_in, *d_out_block, *d_out_naive ;
    cudaMalloc(&d_in, num_bytes);
    cudaMalloc(&d_out_block, num_bytes);
    cudaMalloc(&d_out_naive, num_bytes);
    
    /* fill input matrix with random floats  */
    random_doubles(h_in , dimx*dimy);

    /* copy matrices in device memory */
    cudaMemcpy( d_in, h_in, num_bytes, cudaMemcpyHostToDevice );
    cudaMemcpy( d_out_block, h_out_block, num_bytes, cudaMemcpyHostToDevice );
    cudaMemcpy( d_out_naive, h_out_naive, num_bytes, cudaMemcpyHostToDevice );


    /* set threads and blocks grids */
    int block_rows = threads_per_block/TILE_DIM;
    dim3 grid, block;
    block.x = TILE_DIM;
    block.y = block_rows;
    grid.x = dimx/TILE_DIM;
    grid.y = dimy/TILE_DIM;

    /* cuda events for timing */
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    /* run blocking transpose kernel */
    fast_transpose<<< grid, block >>>(d_in, d_out_block, block_rows);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);
    float bw = 2 * dimx * dimy * sizeof(double) * 1e-6 / ms;
    printf("Fast transpose: \t %lf  \t  %lf \n", ms, bw);

    /* copy the result */
    cudaMemcpy( h_out_block, d_out_block, num_bytes, cudaMemcpyDeviceToHost );

    cudaEventRecord(start);

    /* run naive transpose kernel */
    naive_transpose<<< grid, block >>>(d_in, d_out_naive, block_rows) ;

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float ms_naive = 0;
    cudaEventElapsedTime(&ms_naive, start, stop);
    float bw_naive = 2 * dimx*dimy * sizeof(double) * 1e-6 / ms_naive;
    printf("Naive transpose: \t %lf  \t  %lf \n", ms_naive, bw_naive);

    /* copy result */
    cudaMemcpy( h_out_naive, d_out_naive, num_bytes, cudaMemcpyDeviceToHost) ;

    equality_test(h_out_naive, h_out_block, dimx);
    
    printf("\n ");

    /* deallocate memory */
    free(h_in); free(h_out_naive); free(h_out_block);
    cudaFree(d_in);
    cudaFree(d_out_naive);
    cudaFree(d_out_block);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

}


int main(int argc, char * argv[]){

    int dimx = 8192;
    int dimy = 8192;

    printf(" \t \t \t GPU TIME (ms)     BANDWIDTH (GB/s) ");
    printf("\n\n");
    
    printf("64 threads per block: \n");
    run_kernel(64, dimx, dimy);

    printf("512 threads per block:\n");
    run_kernel(512, dimx, dimy);

    printf("1024 threads per block:\n");
    run_kernel(1024, dimx, dimy);


    return 0;

}










