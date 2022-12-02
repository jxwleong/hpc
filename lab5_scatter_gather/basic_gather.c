#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <mpi.h>
// Example: https://www.eecg.utoronto.ca/~amza/ece1747h/homeworks/examples/MPI/other-examples/pipeline.c
// http://homepages.math.uic.edu/~jan/mcs572/pipelines.pdf
// 2D Array double pointer example

#define MAXPIXEL 256
#define TEMPFILE "2d_mpi.pgm"

typedef enum{false, true} bool;

int main(int argc, char*argv[])
{ 
    int x, y;
    int modulus;
    int start, end;

    int numprocs, rank, namelen, dest=0, tag=0, source;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
    int array[2];
    array[0] = rank*1;
    array[1] = rank*2;

    int global_array[numprocs*2];
    
    printf("Rank %d array[0].address=%p array[1].address=%p\n", rank, &array[0], &array[1]);
    
    MPI_Gather(array, 2, MPI_INT, global_array, 2, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD); // Wait for all process
    
    if (!rank){
        for(int loop = 0; loop < 8; loop++)
            printf("%d  ", global_array[loop]);
    }
    printf("\n");
    MPI_Finalize();  
}