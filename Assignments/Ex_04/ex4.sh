
cd $HOME/parallel_programming/Assignments/Ex_04

module load openmpi

mpicc ex5.c -o ex5.x

for np in 2 4 8 16 20
do
echo "Number of processes: " ${np} >> ex5.out
(mpirun -np ${np} ./ex5.x) >> ex5.out
done

exit

