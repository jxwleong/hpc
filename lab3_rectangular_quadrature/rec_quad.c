#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{ 
    int MASTER = 0;
    int n, i, split_per_procs, start, end;
    int numprocs, rank, namelen, dest=0, tag=0, source;
	int BUFSIZE = 50;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Status mpi_status;
    float local_sum = 0, global_sum = 0;
    int x;
    n = 16;

    
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
    


   
    if (rank == MASTER){
        printf("Enter x: ");
        scanf("%d", &x);  
        printf("%d\n", x);
    } 

    MPI_Finalize();  
    


}