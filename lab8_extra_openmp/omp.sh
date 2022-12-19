gcc -fopenmp -o omp omp.c  -lm

OMP_NUM_THREADS=8 ./omp