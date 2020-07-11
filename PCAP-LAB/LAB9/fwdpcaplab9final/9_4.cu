#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>

__global__ void func4(int* a, int* t)
{
	int n = threadIdx.x;
	int m = blockIdx.x;
	int size = blockDim.x;
	int size1 = gridDim.x;

	int dx = a[m*size+n];
	if ((n != 0) && (m != 0) && (n != size-1) && (m != size1-1))
	{
		int gx;
		int ax = 0;
		int fac = 1;
		while (dx != 0)
		{
			gx = dx%2;
			if (gx == 0)
			{
				ax += fac;
			}
			fac *= 10;
			dx /= 2;
		}

		t[m*size+n] = ax;
	}
	else
	{
		t[m*size+n] = dx;
	}
}

int main(void)
{
	int *a,*t,m,n,i,j;
	int *d_a,*d_t;

	printf("Enter value of m: ");
	scanf("%d",&m);
	printf("Enter value of n: ");
	scanf("%d",&n);

	int size = sizeof(int)*m*n;
	a = (int*)malloc(m*n*sizeof(int));
	t = (int*)malloc(m*n*sizeof(int));

	printf("Enter input matrix:\n");
	for (i = 0; i < m*n; i++)
	{
		scanf("%d",&a[i]);
	}

	cudaMalloc((void**)&d_a,size);
	cudaMalloc((void**)&d_t,size);

	cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
	func4<<<m,n>>>(d_a,d_t);
	cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);

	printf("The result vector is:\n");
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%d\t",t[i*n+j]);
		}
		printf("\n");
	}

	getchar();
	cudaFree(d_a);
	cudaFree(d_t);
	return 0;
}