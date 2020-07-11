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
		printf("sending %d to a process\n",n++);
		MPI_Send(&n,1,MPI_INT,1,0,MPI_COMM_WORLD);
		printf("sent %d to a process\n",n);
		sleep(1);
		printf("sending %d to a process\n",n++);
		MPI_Send(&n,1,MPI_INT,1,0,MPI_COMM_WORLD);
		printf("sent %d to a process\n",n);
	}
	else if(rank == 1){
		printf("receive waiting to receive n from a process\n");
		MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		printf("Received n: %d\n",n);
		printf("receive waiting to receive n from a process\n");
		MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		printf("Received n: %d\n",n);
	}

	MPI_Finalize();
	return 0;
}