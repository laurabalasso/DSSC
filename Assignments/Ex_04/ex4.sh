
cd $HOME/parallel_programming/Assignments/Ex_04

module load openmpi

mpicc ex4.c -o ex4.x

for np in 2 4 8 16 20
do
echo "Number of processes: " ${np} >> ex4.out
(mpirun -np ${np} ./ex4.x) >> ex4.out
done

exit

