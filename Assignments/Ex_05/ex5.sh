/bin/hostname

cd $HOME/parallel_programming/Assignments/Ex_05

module load openmpi/1.8.3/intel/14.0

mpicc ex5_array.c -o ex5.x

(mpirun -np 4 ex5.x) >> ex5.out


exit
