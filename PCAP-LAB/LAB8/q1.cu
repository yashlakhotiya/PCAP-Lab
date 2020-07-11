%%cu
#include <stdio.h>

__global__ void stringCopy(char *S, char  *B, int N){
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	int offset = id*N;
	for(int i=0; i<N; i++){
		B[offset + i] = S[i];
	}
}

int main(){
	char S[1000] = "hello",B[1000];
	int N = 3;
	/*printf("enter string\n");
	scanf("%s",S);
	printf("enter N\n");
	scanf("%d",&N);*/
	char *d_a, *d_b;

	int size = sizeof(char);

	cudaMalloc((void **)&d_a,size * strlen(S));
	cudaMalloc((void **)&d_b,size * N * strlen(S));

	cudaMemcpy(d_a,S, size*strlen(S), cudaMemcpyHostToDevice);

	stringCopy<<<N,1>>>(d_a,d_b,strlen(S));

	cudaMemcpy(B,d_b,size*strlen(S)*N,cudaMemcpyDeviceToHost);

	printf("str S: %s\n",S);
	printf("str B: %s\n",B);
	printf("\n");

	cudaFree(d_a);
	cudaFree(d_b);
	return 0;
}