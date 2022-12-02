mpicc 2d_mpi.c -o 2d_mpi -lm -g -O0
# mpicc for c
# mpiCC for cpp
mpirun -np 4 ./2d_mpi