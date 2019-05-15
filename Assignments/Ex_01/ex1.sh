cd $HOME/parallel_programming/Assignments/Ex_01/

module load intel

icc -qopenmp ex1.c -o ex1.x
icc -qopenmp ex1_critical.c -o ex1_crit.x
icc -qopenmp ex1_reduction.c -o ex1_red.x

for proc in 1 2 4 8 16 20
do
export OMP_NUM_THREADS=${proc}
./ex1.x >> ex1_atomic.out
./ex1_crit.x >> ex1_crit.out
./ex1_red.x >> ex1_red.out

done

exit

