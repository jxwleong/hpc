#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <mpi.h>

// Example: https://www.eecg.utoronto.ca/~amza/ece1747h/homeworks/examples/MPI/other-examples/pipeline.c
// http://homepages.math.uic.edu/~jan/mcs572/pipelines.pdf
#define MAXPIXEL 256
#define TEMPFILE "temp.pgm"

int xsize, ysize;

int f(int x, int y)
{ 
    double divisor = (M_PI*xsize/60);
    double zx = (double)x/divisor;
    double zy = (double)y/divisor;
    return (sin(zx)+cos(zy))*(MAXPIXEL/4)+(MAXPIXEL/2);
}

int main(int argc, char*argv[])
{ 
    int x, y;
    int start, end;
    int xsplit_per_procs, ysize_for_each_proc;
    int **picture;
    FILE *outfile;

    int numprocs, rank, namelen, dest=0, tag=0, source;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);


    if (!rank) {
        printf("How wide and tall should the picture be? ");fflush(stdout);
        scanf("%d %d", &xsize, &ysize);
        picture =(int**)malloc(sizeof(int*)*ysize);
    }
    MPI_Bcast(&xsize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ysize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    xsplit_per_procs = xsize/numprocs;
    // x start from 1 ends with xsize
    // Assume xsize, ysize=100
    // For process0
    // start=0
    // end=xsplit_per_proc=24
    start = (xsplit_per_procs*rank);
    end = (xsplit_per_procs*rank) + xsplit_per_procs -1;

    //printf('Y: %d\n', end);
    for (y=0; y<ysize; y++)
    { 
        //printf("\n\n\nRANK: %d\n", rank);
        picture[y] = (int*)malloc(sizeof(int)*xsize);
        for (x=start; x<=end; x++)
        {
            //printf("%d\n", x);
            picture[y][x] = f(x,y);
        }
        //printf("%d\n", xsplit_per_procs);
    }

   
    MPI_Barrier(MPI_COMM_WORLD); // Wait for all process
    if (!rank){
        outfile=fopen(TEMPFILE,"w");
        fprintf(outfile,"P2\n%d %d %d\n",xsize, ysize, MAXPIXEL);
        for(y=0; y<ysize; y++)
        for (x=0; x<xsize; x++)
        fprintf(outfile,"%d\n", picture[y][x]);
        fclose(outfile);
        system("display "TEMPFILE);
    }
    MPI_Finalize();  
}