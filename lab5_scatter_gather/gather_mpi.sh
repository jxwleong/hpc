mpicc gather_mpi.c -o gather_mpi -lm -g -O0
# mpicc for c
# mpiCC for cpp
mpirun -np 4 ./gather_mpi