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

	int arr[n],scat_arr[n/size],gath_arr[n],all_to_all_arr[n],sum[n];

	if(rank == 0){
		printf("enter array\n");
		for(int i=0; i<n; i++){
			scanf("%d",&arr[i]);
		}
	}

	MPI_Scatter(arr,n/size,MPI_INT,scat_arr,n/size,MPI_INT,0,MPI_COMM_WORLD);

	for(int i=0; i<n/size; i++){
		scat_arr[i] += rank;
	}

	MPI_Allgather(scat_arr,n/size,MPI_INT,gath_arr,n/size,MPI_INT,MPI_COMM_WORLD);

	MPI_Alltoall(gath_arr,n/size,MPI_INT,all_to_all_arr,n/size,MPI_INT,MPI_COMM_WORLD);

	MPI_Reduce(all_to_all_arr,sum,n,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	if(rank == 0){
		printf("sum of all all_to_all_arr is:\n");
		for(int i=0; i<n; i++){
			printf("%d\t",sum[i]);
		}
	}


	MPI_Finalize();
	return 0;
}