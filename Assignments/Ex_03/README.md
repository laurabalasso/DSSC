# Exercise 3

### MPI Pi approximation

To reproduce the results in Ulysses, clone the Github repository and use the following command:

qsub -l -q reserved3 nodes=2:ppn=20,walltime=00:15:00 parallel_programming/Assignments/Ex_03/ex3.sh

A file .out will be produced in the folder Ex_03 with Pi approximation and execution time for different numbers of MPI processes, 
from 1 up to 40.
