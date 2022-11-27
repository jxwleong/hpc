#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <mpi.h>

// Example: https://www.eecg.utoronto.ca/~amza/ece1747h/homeworks/examples/MPI/other-examples/pipeline.c
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
    int ysplit_per_procs, ysize_for_each_proc;
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

    ysplit_per_procs = ysize/numprocs;

    MPI_Bcast(&xsize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ysize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    ysize_for_each_proc = (rank*ysplit_per_procs) + ysplit_per_procs;
    for (y=0; y<ysize_for_each_proc; y++)
    { 
        picture[y] = (int*)malloc(sizeof(int)*xsize);
        for (x=0; x<xsize; x++)
            picture[y][x] = f(x,y);
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
}