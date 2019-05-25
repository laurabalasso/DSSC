# Exercise 6

### Cuda Fast Transpose

This folder contains an implementation of the fast transpose in Cuda.

To reproduce the results in Ulysses, clone the Github repository and use the following command:

qsub -l -q gpu nodes=1:ppn=20,walltime=00:10:00 parallel_programming/Assignments/Ex_06/fast_transpose.sh

A file .out will be produced in the folder Ex_06 containing the gpu time and the associated bandwidth for both the naive and the optimized
kernel, with 64, 512 and 1024 threads per block.
