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
	printf("Process %d on %s out of %d\n", rank, processor_name,
			numprocs);
	*/
	if (rank == 0){
		//printf("Process %d on %s out of %d\n", rank, processor_name,
		//	numprocs);

		for (int source_rank = 1; source_rank < numprocs ;source_rank++){
			MPI_Recv(&message, BUFSIZE, MPI_CHAR, source_rank, tag, MPI_COMM_WORLD, &mpi_status);
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
