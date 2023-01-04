gcc -fopenmp -o omp_eq omp_eq.c  -lm

OMP_NUM_THREADS=8 ./omp_eq