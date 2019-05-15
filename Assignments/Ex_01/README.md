# Exercise 1

### Pi approximation using OpenMP

To reproduce the results in Ulysses, clone the Github repository and use the following command:

qsub -l -q reserved3 nodes=1:ppn=20,walltime=00:10:00 parallel_programming/Assignments/Ex_01/ex1.sh

Three files .out will be produced in the folder Ex_01, one for each version of the code. 
Each of them will contain the Pi approximation and the time to solution for 1, 2, 4, 8, 16 and 20 threads. 
