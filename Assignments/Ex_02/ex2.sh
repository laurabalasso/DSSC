/bin/hostname

cd $HOME/parallel_programming/Assignments/Ex_02/

export OMP_NUM_THREADS=5

module load intel

icc -qopenmp ex2.c -o ex2

./ex2 >> ex2.out

exit

