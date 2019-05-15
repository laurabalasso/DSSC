# Exercise 2

### OpenMP schedules visualisation

To reproduce the results in Ulysses, clone the Github repository and use the following command:

qsub -l -q reserved3 nodes=1:ppn=20,walltime=00:10:00 parallel_programming/Assignments/Ex_02/ex2.sh

A file .out will be produced in the folder Ex_02 with a graphical visualisation of the OpenMP threads 
schedule in static and dynamic mode and using different chunck sizes. 
