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
		printf("enter n: n should be greater than or equal to and multiple of number of process\n");
		scanf("%d",&n);
	}

	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

	printf("n received in process %d as n: %d\n",rank,n);

	int arr[n],A[n/size],C[n/size],B[n/size];

	if(rank == 0){
		printf("enter array\n");
		for(int i=0; i<n; i++){
			scanf("%d",&arr[i]);
		}
	}

	MPI_Scatter(arr,n/size,MPI_INT,A,n/size,MPI_INT,0,MPI_COMM_WORLD);

	MPI_Alltoall(A,1,MPI_INT,B,1,MPI_INT,MPI_COMM_WORLD);

	MPI_Scan(A,C,3,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	if(rank == 2){
		printf("\nfinal array in process %d is\n",rank=1);
		
		for(int i=0; i<n/size; i++){
			printf("%d\t",C[i]);
		}
	}



	MPI_Finalize();
	return 0;
}