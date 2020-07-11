#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1000

int main(int argc, char *argv[]){
	MPI_Init(&argc,&argv);
	int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int n;
	MPI_Status status;

	if(rank == 0){
		printf("enter n\n");
		scanf("%d",&n);
	}

	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	printf("n received in process %d as n: %d\n",rank,n);


	MPI_Finalize();
	return 0;
}