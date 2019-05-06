#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void print_usage(int * a , int N, int nthreads){
  int tid,i;
  for(tid = 0; tid < nthreads; ++tid){
    fprintf(stdout, "%d: ", tid);

    for(i = 0; i < N; ++i){
      if(a[i] == tid) fprintf(stdout, "*");
      else fprintf(stdout, " ");
    }
    printf("\n");
  }

}

int main(int argc, char* argv[]){

  int chunck_size;
  const int N = 100;
  int a[N];
  int nthreads;
  int thread_id;

  /*Serial version*/
  printf("Serial: \n");
  thread_id = 0;
  nthreads = 1;
  int i;
  for(i = 0; i < N ; i++){
    a[i] = thread_id;
  }

  print_usage(a, N, nthreads);

  /* Static threads schedule, with chunck size = 1 */

  chunck_size = 1;
  nthreads = 5;
  #pragma omp parallel num_threads(nthreads)
  {
    thread_id = omp_get_thread_num();
    int i;

  #pragma omp for schedule(static , chunck_size) private(i)
    for(i = 0; i < N; i++)
      a[i] = thread_id;

}
  printf("Static schedule: \n");
  print_usage(a, N, nthreads);

   /* Static schedule, with chunck size = 10 */

   chunck_size = 10;
   #pragma omp parallel num_threads(nthreads)
   {
     int thread_id = omp_get_thread_num();
     int i;

     #pragma omp for schedule( static , chunck_size)                              
     for(i = 0; i < N; i++)
       a[i] = thread_id;


}

   printf("Static schedule with chunck size = 10:\n");
   print_usage(a, N, nthreads);

 /* Dynamic schedule, with chunck size = 1 */

   chunck_size = 1;
   #pragma omp parallel num_threads(nthreads)
   {
     int thread_id = omp_get_thread_num();
     int i;

     #pragma omp for schedule(dynamic , chunck_size)
     for(i = 0; i < N; i++)
       a[i] = thread_id;

   }

   printf("Dynamic schedule with chunck size = 1:\n");
   print_usage(a, N, nthreads);
 /* Dynamic schedule, with chunck size = 10 */

   chunck_size = 10;
   #pragma omp parallel num_threads(nthreads)
   {
     int thread_id = omp_get_thread_num();
     int i;

     #pragma omp for schedule(dynamic , chunck_size)
     for(i = 0; i < N; i++)
       a[i] = thread_id;


}

   printf("Dynamic schedule with chunck size = 10:\n");
   print_usage(a, N, nthreads);


}

