mpicc rec_quad.c -o rec_quad -lm
# mpicc for c
# mpiCC for cpp
mpirun -np 4 ./rec_quad