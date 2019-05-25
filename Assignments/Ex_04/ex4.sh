/bin/hostname

cd $HOME/parallel_programming/Assignments/Ex_04

module load openmpi

mpicc ex4.c -o ex4.x

mpirun -np 4 ./ex4.x >> ex4.out

exit

