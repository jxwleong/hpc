#include <stdio.h>
#include <math.h>

#define START (M_PI/2)
#define END (M_PI*2)

double f(double x)
{ 
    return sin(x/2)+1;
}

int main(int argc, char *argv[])
{ 
    double total = 0, x;
    int partitions;
    double slice;
    printf("How many partitions? "); fflush(stdout);
    scanf("%d", &partitions);
    slice = (END-START)/partitions;
    for (x = START + (slice/2); x < END; x = x + slice)
        total = total + f(x);
    total = total * slice;
    printf("The integration is %1.20f\n", total);
}