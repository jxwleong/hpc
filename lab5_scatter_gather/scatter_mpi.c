#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>

#define BUFSIZE 256

int main(int argc, char *argv[])
{ 
    int *each_data, each_result;
    int *global_data, global_result;
    int size, each_size, i, modulus;
    int numprocs, rank, namelen, dest=0, tag=0, source;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    FILE *infile;
    char buf[BUFSIZE];

    // Init MPI
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);


    if (!rank){
        // Hardcode file name, uncomment bottom part
        // comment out the next 3 lines
        //strncpy(buf, "lab06dat1.txt", 100);
        printf("What is the name of the data file?\n");
        fgets(buf, BUFSIZE, stdin);
        buf[strlen(buf)-1]='\0'; // Remove the carriage return from the file name
        infile = fopen(buf,"r");
        
        if (infile==NULL)  
        { 
            perror ("Opening file");
            exit(1);
        }
        fscanf(infile,"%d", &size); // Find out how big the data is
        printf("Total size = %d\n", size);

        global_data = (int *)malloc(sizeof(int)*size); // Allocate the space
        // printf
        for (i=0; i<size; i++) // Read the data
        { 
            fscanf(infile,"%d", &global_data[i]);
            //printf("%d ",global_data[i]);
        }
    }



    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // Important to init the result=0!!!
    // Else inconsistent value will occurs
    // https://stackoverflow.com/q/56693524
    each_result=0;
    global_result=0;
    each_size = (size/numprocs);  
    modulus = size%numprocs;
    each_data = (int *)malloc(sizeof(int)*each_size); // Allocate the space
    //printf("\n\nsize=%d, eachsize=%d", size, each_size);
    MPI_Scatter(global_data, each_size, MPI_INT,    
                each_data, each_size, MPI_INT,  
                0, MPI_COMM_WORLD);

    
    //printf("\n\nDATA for rank (3)\n", rank);
    for (i=0; i<each_size; i++){
        each_result += each_data[i];  
    } 
    printf("\nRANK[%d]'s result= %d\n", rank, each_result);
    //printf("\nRank[%d]  First=%d, Last=%d\n", rank, each_data[0], each_data[each_size-1]);
    MPI_Reduce(&each_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 

    //MPI_Barrier(MPI_COMM_WORLD); // Wait for all process

    if (!rank){
        // Calculate the left over if there are any...
        //printf("\n\nModulo: %d, i: %d", modulus, each_size*numprocs);
        if (modulus!=0){
            //printf("\b Result before add: %d", global_result);
            for (i=0; i<modulus; i++)
                global_result += global_data[each_size*numprocs+i];
        } 
        printf("\n\nResult: %d\n",global_result);
    }

    MPI_Finalize();  
}