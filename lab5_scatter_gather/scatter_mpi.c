#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>

#define BUFSIZE 256

int main(int argc, char *argv[])
{ 
    int *each_data, each_result;
    int size, each_size, i;
    int numprocs, rank, namelen, dest=0, tag=0, source;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    FILE *infile;
    char buf[BUFSIZE];

    if (!rank){
        printf("What is the name of the data file?\n");
        fgets(buf, BUFSIZE, stdin);
        buf[strlen(buf)-1]='\0'; // Remove the carriage return from the file name
        infile = fopen(buf,"r");
        
        if (infile==NULL)  
        { 
            perror ("Opening file");
            exit(1);
        }
    }


    // Init MPI
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);

    fscanf(infile,"%d", &size); // Find out how big the data is
    printf("Size = %d\n",size);
    each_data = (int *)malloc(sizeof(int)*size); // Allocate the space


    for (i=0; i<size; i++) // Read the data
    { 
        fscanf(infile,"%d",&each_data[i]);
        printf("%d ",each_data[i]);
    }
    printf("\n");
    result = 0;
    for (i=0; i<size; i++)
        result += data[i];
    printf("Result: %d\n",result);
}