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
	int n,buf_size = BUFFER_SIZE,buffer[BUFFER_SIZE];
	MPI_Buffer_attach(buffer,buf_size);
	MPI_Status status;

	if(rank == 0){
		printf("enter n in host\n");
		scanf("%d",&n);

		printf("sending n to all processes\n");
		for(int i=1; i<size; i++){
			MPI_Bsend(&n,1,MPI_INT,i,0,MPI_COMM_WORLD);
			printf("sent n: %d from 0 to %d\n",n,i);
		}
	}
	else{
		MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		printf("Received n: %d in %d from 0\n",n,rank);
	}

	MPI_Buffer_detach(buffer,&buf_size);
	MPI_Finalize();
	return 0;
}