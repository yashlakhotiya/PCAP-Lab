#include <stdio.h>

__global__ void MatMul(int *a, int *b, int *t, int m0, int n0, int m1, int n1)
{
	int nthColumn = threadIdx.x, mthRow = blockIdx.x;

	int temp = 0;
	for( int i = 0; i < n0 ; i++ )
	{
		temp += a[mthRow*n0 + i] * b[nthColumn + i*n1];
		// printf("%d %d: %d %d\n", mthRow, nthColumn, a[mthRow*n0 + i], b[nthColumn + i*n1]);
	}
	t[mthRow*n1 + nthColumn] = temp;

	


}
int main() {
	int *a, *b, *t, m0, n0, m1, n1, i, j;
	int *d_a, *d_b, *d_t;

	printf("Enter value of m0\n"); scanf("%d", &m0);
	printf("Enter value of n0\n"); scanf("%d", &n0);
	int size0 = sizeof(int)*m0*n0;

	printf("Enter value of m1\n"); scanf("%d", &m1);
	printf("Enter value of n1\n"); scanf("%d", &n1);
	int size1 = sizeof(int)*m1*n1;

	int sizet = sizeof(int)*m0*n1;

	if(n0!=m1)
	{
		printf("Invalid matrix dimensions.\n");
		exit(0);
	}

	a = (int *)malloc(m0*n0*sizeof(int));
	b = (int *)malloc(m1*n1*sizeof(int));
	t = (int *)malloc(m0*n1*sizeof(int));


	printf("Enter input matrix A\n");
	for(i=0; i< m0*n0; i++)
		scanf("%d", &a[i]);

	printf("Enter input matrix B\n");
	for(i=0; i< m1*n1; i++)
		scanf("%d", &b[i]);


	cudaMalloc((void**)&d_a, size0);
	cudaMalloc((void**)&d_b, size1);
	cudaMalloc((void**)&d_t, sizet);

	cudaMemcpy(d_a, a, size0, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, size1, cudaMemcpyHostToDevice);

	MatMul<<<m0,n1>>>(d_a, d_b, d_t, m0, n0, m1, n1);

	cudaMemcpy(t, d_t, sizet, cudaMemcpyDeviceToHost);

	printf("result vector:\n");
	for(i=0; i<m0; i++) {
		for(j =0; j<n1; j++)
			printf("%d ", t[i*n1+j]);
		printf("\n");
	}

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_t);
	return 0;



}

// 2 3 3 2 1 2 3 4 5 6 1 2 3 4 5 6