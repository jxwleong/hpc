#include <stdio.h>
#include <mpi.h>

/* Calculate the sum for i=1..n of 1/n */
/*
printf("Please enter n: "); fflush(stdout); scanf("%d", &n);
for (i=1; i<=n; i++)
    total += 1.0/i;
printf("The sum of the reciprocal from 1 to %d is %f.\n", n, total);
*/
int main(int argc, char *argv[])
{ 
    int MASTER = 0;
    int n, i, split_per_procs, start, end;
    int numprocs, rank, namelen, dest=0, tag=0, source;
	int BUFSIZE = 50;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Status mpi_status;
    float local_sum = 0, global_sum = 0;

    n = 16;
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
    split_per_procs = n/numprocs; 

    // Splitting Method
    start = (rank * split_per_procs) + 1;
    end = (rank+1) * split_per_procs;  
    
    for (i=start; i<=end; i++)
        local_sum += 1.0/i;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == MASTER){
        printf("[Splitting] The sum of the reciprocal from 1 to %d is %f.\n", n, global_sum);
    } 
	
    MPI_Barrier(MPI_COMM_WORLD);

    // Intersperse
    local_sum = 0;
    global_sum = 0;
    for (int i=rank+1; i<=n; i=i+numprocs){
        local_sum += 1.0/i;
    }
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == MASTER){
        printf("[Intersperse] The sum of the reciprocal from 1 to %d is %f.\n", n, global_sum);
    } 

    MPI_Finalize();

}