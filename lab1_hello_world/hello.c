#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{ 
	// Sent to rank 0 so dest=0
	// Only one type of message so tag=0
	int numprocs, rank, namelen, dest=0, tag=0, source;
	int BUFSIZE=50;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	char message[BUFSIZE];
	MPI_Status mpi_status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
	/*
	printf("Process %d on %s out of %d\n", rank, processor_name,lab1_hello_world/hello.c
			numprocs);
	*/
	if (rank == 0){
		//printf("Process %d on %s out of %d\n", rank, processor_name,
		//	numprocs);
		// numproces -1, to offset rank0, else will keep waiting for rank0 receive
		for (int rank_count = 0; rank_count < numprocs-1 ;rank_count++)
		{
			MPI_Recv(&message, BUFSIZE, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &mpi_status);
			printf("%s", message);

		}
	}
	else{
		//printf("LOL\n");
		sprintf(message, "Greeting from rank %d\n", rank);
		//printf("%s", message);
		MPI_Send(&message, BUFSIZE, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
