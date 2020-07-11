#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	MPI_Init(&argc,&argv);
	int rank,size;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int n=4, A[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	int k=0;
	int s0 = n;
	int s1 = (n*n-n)/2;
	int arr[s1];
	for(int i=0; i<s1; i++) arr[i]=0;
	int sum = 0;
	MPI_Status status;

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			int val = A[i*n+j];
			if(i == j && rank == 0){
				arr[k++] = val;
			}
			else if(i < j){
				if(rank == 0){
					MPI_Send(&val,1,MPI_INT,1,0,MPI_COMM_WORLD);
				}
				else{
					if(rank == 1) MPI_Recv(&arr[k++],1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
				}
			}
			else if(i > j){
				if(rank == 0){
					MPI_Send(&val,1,MPI_INT,2,0,MPI_COMM_WORLD);
				}
				else{
					if(rank == 2) MPI_Recv(&arr[k++],1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
				}
			}
		}
	}
	if(rank == 0){
		for(int i=0; i<s0; i++){
			sum += arr[i];
		}
		printf("\np0: %d\n",sum);
	}
	else if(rank == 1){
		for(int i=0; i<s1; i++){
			sum += arr[i];
		}
		printf("\np1: %d\n",sum);
	}
	else if(rank == 2){
		for(int i=0; i<s1; i++){
			sum += arr[i];
		}
		printf("\np2: %d\n",sum);
	}
	MPI_Finalize();
	return 0;
}