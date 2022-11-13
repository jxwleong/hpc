#include <stdio.h>
#include <mpi.h>
#include <math.h>
// https://github.com/cppf/rectangular-integration/blob/main/main.cpp

double f(double x){
    return sin(x/2) + 1;
}

int main(int argc, char *argv[])
{ 
    int MASTER = 0;
    int n=4, i, split_per_procs, start, end;
    int numprocs, rank, namelen, dest=0, tag=0, source;
	int BUFSIZE = 50;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Status mpi_status;
    double local_sum = 0.0, global_sum = 0.0;
    double total = 0.0;
    double w = 0.0;
    int x;
    //n = 1000;

    double starttime=0.0, endtime=0.0;
    // Without MPI, just run '-np 1'
    /*
    printf("Enter number of partitions: ");
    fflush(stdout);  // need this else the msg above will show after user input
    scanf("%d", &n);

    starttime = MPI_Wtime();
    for (i=0; i<n; i++){
        x = (M_PI/2) + (i + 1/2);
        total += f(x);
    }
    w = ((2*M_PI) - (M_PI/2)) /n;
    total = total * w;
    endtime = MPI_Wtime();
    printf("Total = %lf with %d partitions. Time taken %lf seconds \n", total, n, endtime - starttime);
    */
    // With MPI
    MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
    
    if (!rank) {
        printf("Enter number of partitions: ");
        fflush(stdout);  // need this else the msg above will show after user input
        scanf("%d", &n);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (!rank) 
        starttime = MPI_Wtime();
    for (i=rank; i<n; i+=numprocs){
        x = (M_PI/2) + (i + 1/2);
        local_sum += f(x); 
    }
    //printf("\nRank %d, local sum: %lf", rank, local_sum);
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
    w = ((2*M_PI) - (M_PI/2)) /n;
    global_sum = global_sum * w;

    MPI_Barrier(MPI_COMM_WORLD); // Wait for all process
    //printf("\nGlobal sum: %lf", global_sum);
    if (!rank){
        endtime = MPI_Wtime();
        printf("\nTotal = %lf with %d partitions. Time taken %lf seconds \n", global_sum, n, endtime - starttime);
    }
    MPI_Finalize();  
}