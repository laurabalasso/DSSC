cd $HOME/parallel_programming/Assignments/Ex_06

module load cudatoolkit/10.0

nvcc fast_transpose.cu -o transpose.x

./transpose.x >> transpose.out
