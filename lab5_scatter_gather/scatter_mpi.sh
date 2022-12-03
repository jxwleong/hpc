mpicc scatter_mpi.c -o scatter_mpi -lm -g -O0
# mpicc for c
# mpiCC for cpp
mpirun -np 4 ./scatter_mpi