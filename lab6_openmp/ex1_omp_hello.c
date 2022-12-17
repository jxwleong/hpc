#include <stdio.h>
#include <omp.h>

int main() {
    int i;
    printf("Hello World\n");
    #pragma omp parallel for
    for(i=0;i<6;i++)
        printf("Thread: %d, Iter:%d\n",omp_get_thread_num(), i);

    printf("GoodBye World\n");

}