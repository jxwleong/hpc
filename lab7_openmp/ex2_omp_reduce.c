#include <stdio.h>
#include <omp.h>
/*ORIGINAL CODE FROM QUESTION
Problem: The data will be clobbered as all the processes might be accessing the
'sum' at the same time.
float dot_prod(float* a, float* b, int N)
{
    float sum = 0.0;
    #pragma omp parallel for shared(sum)
    for(int i=0; i<N; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

*/

/*
Solution: Use reduction
Src: https://stackoverflow.com/a/16544624
*/
float dot_prod(float* a, float* b, int N)
{
    float sum = 0.0;
    #pragma omp parallel for reduction (+:sum)
    for(int i=0; i<N; i++) {
        sum += a[i] * b[i];
        printf("[T%d][A:%f][B:%f][N:%d]",omp_get_thread_num(), a[i], b[i], N);
        printf("sum=%f\n", sum);
    }
    return sum;
}


int main() {
    int i=5;
    float a[5]={1, 2, 3, 4, 5}; 
    float b[5]={5, 7, 8, 9, 10};
    printf("Product for %d iter: %f\n", i, dot_prod(a,b,i));

}