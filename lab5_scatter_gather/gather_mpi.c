#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <mpi.h>

#define BUFSIZE 256

int main(int argc, char *argv[])
{ 
    int i, size, each_size;
    int modulus;
    long *int_buf, *local_buf;
    char filename[BUFSIZE];
    FILE *outfile;
    int numprocs, rank, namelen, dest=0, tag=0, source;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    // Init MPI
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);

    if (!rank){
        // Hardcode the file name and size
        // Uncomment the next 4 lines to ask for user input
        //strncpy(filename, "ran_int_out.txt", 100);
        //size=1750;
        printf("What is the file name you want to write to? \n");
        scanf("%s", filename);
        printf("How many long integers to generate? \n");
        scanf("%d", &size);
        outfile = fopen(filename,"w");
        if (!outfile)
        { 
            fprintf(stderr,"Unable to open %s for writing.\n", filename);
            exit(1);
        }
        fprintf(outfile,"%d\n",size);
        srandom(time(0));
    }
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //printf("\nsize: %d", size);
    int_buf= (long*)malloc(sizeof(long)*size);
    local_buf= (long*)malloc(sizeof(long)*size);
    each_size=size/numprocs;
    modulus = size%numprocs;
    //printf("\nEach size: %d", each_size);
    //printf("\n\nALOHA");
    // Each proc creating their own subarray
    for (i=0; i<each_size; i++){
        //printf("\n RANK[%d], local_buf[%d]=%d", rank, i, local_buf[i]);
        local_buf[i] = random();
    }
    
    //printf("\neach_size= %d", each_size);
    MPI_Gather(local_buf, each_size, MPI_LONG,
                int_buf, each_size, MPI_LONG,
                0, MPI_COMM_WORLD);

    if (!rank){
        if (modulus!=0){
            for (i=each_size*numprocs; i<size; i++){
                //printf("\n[Modulus], I=%d", i);
                int_buf[i] = random();
                //printf("\n[AFTER][Modulus], int_buf[%d]=%ld", i, int_buf[i]);
            }
        }
        for (i=0; i<size; i++)
            fprintf(outfile,"%ld\n", int_buf[i]);
        fclose(outfile);
    }
    MPI_Finalize();  
}