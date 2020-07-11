#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<stdio.h>
#include<stdlib.h>

__global__ void addmat(int *a, int *b, int *c)
{
	int n= threadIdx.x,m=blockIdx.x, size=blockDim.x;
	c[m*size+n]=a[m*size+n]+b[m*size+n];
}
__global__ void addrow (int *A, int *B, int *C,int n) {
    int idx = threadIdx.x;
    printf("idx = %d\n", idx);
    for (int i = 0; i < n; ++i) {
        C[i + n * idx] = A[i + n * idx] + B[i + n * idx];
    }
}
__global__ void addcol(int *A, int *B, int *C,int m) {
    int idx = threadIdx.x;
    int x=blockDim.x;
    printf("idx = %d\n", idx);
    for (int i = 0; i < m; ++i) {
        C[ i*x + idx] = A[ i*x + idx] + B[ i*x + idx];
				
    }
}



int main(void)
{
	int a[8]={1,2,3,4,5,6,1,2},b[8]={1,2,3,4,5,6,1,2},*c,*c1,*c2,m=4,n=2,i,j;
	int *d_a,*d_b,*d_c,*d_c1,*d_c2;
	

	int size=sizeof(int)*m*n;
	
	c=(int*)malloc(m*n*sizeof(int));
	c1=(int*)malloc(m*n*sizeof(int));
	c2=(int*)malloc(m*n*sizeof(int));


	cudaMalloc((void**)&d_a,size);
	cudaMalloc((void**)&d_b,size);
	cudaMalloc((void**)&d_c,size);
	cudaMalloc((void**)&d_c1,size);
	cudaMalloc((void**)&d_c2,size);
	
	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);
  addrow<<<1, m>>>(d_a, d_b, d_c,n);	
	cudaMemcpy(c,d_c,size,cudaMemcpyDeviceToHost);

	printf("Result matrix using computation using each row is:\n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
			printf("%d\t",c[i*n+j]);
		printf("\n");
	}


	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);

	addcol<<<1,n>>>(d_a,d_b,d_c2,m);
	cudaMemcpy(c2,d_c2,size,cudaMemcpyDeviceToHost);
	
	
	printf("Result matrix using computation using each column is:\n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
			printf("%d\t",c2[i*n+j]);
		printf("\n");
	}


	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);

	addmat<<<m,n>>>(d_a,d_b,d_c1);
	cudaMemcpy(c1,d_c1,size,cudaMemcpyDeviceToHost);
	
	
	printf("Result matrix using computation using each element is:\n");
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
			printf("%d\t",c1[i*n+j]);
		printf("\n");
	}
	getchar();
	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
	cudaFree(d_c1);
	return 0;


}
