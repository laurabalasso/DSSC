/bin/hostname

cd $HOME/parallel_programming/Assignments/Ex_03

module load openmpi

mpicc ex3.c -o ex3.x

for np in 2 4 8 16 20 24 32 36 40 
do
    echo "Number of processes: " ${np} >> ex3.out
    (mpirun -np ${np} ./ex3.x) >> ex3.out
done


exit






