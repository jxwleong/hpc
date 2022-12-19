#include <stdio.h>
#include <math.h>
#include <omp.h>

#define START (M_PI/2)
#define END (M_PI*2)

double f(double x)
{ 
    return sin(x/2)+1;
}

int main(int argc, char *argv[])
{ 
    double total = 0, x;
    int partitions, i=0;
    double slice;
    printf("How many partitions? "); fflush(stdout);
    scanf("%d", &partitions);
    slice = (END-START)/partitions;

    #pragma omp parallel for
    for (i=0; i<partitions; i++){
        x = START  + (slice/2) + i*slice;
        total = total + f(x);
        printf("\n[T%d] x=%f, i=%d, total=%f",omp_get_thread_num(), x, i, total);
    }
    total = total * slice;
    printf("\nOUTER TOTAL: %f", total);
    printf("\nThe integration is %1.20f\n", total);
}