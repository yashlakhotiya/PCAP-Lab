#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	MPI_Init(&argc,&argv);
	int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int n;
	MPI_Status status;

	if(rank == 0){
		printf("enter n in host\n");
		scanf("%d",&n);

		printf("sending n to all processes\n");
		for(int i=1; i<size; i++){
			MPI_Send(&n,1,MPI_INT,i,0,MPI_COMM_WORLD);
			printf("sent n: %d from 0 to %d\n",n,i);
		}
	}
	else{
		MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		printf("Received n: %d in %d from 0\n",n,rank);
	}
	MPI_Finalize();
	return 0;
}