%%cu
#include <stdio.h>
#include <stdlib.h>


__global__ void transpose(int *a, int *t){
    int n = threadIdx.x;
    int m = blockIdx.x;
    int size = blockDim.x;
    int size1 = gridDim.x;
    t[n*size1+m] = a[m*size+n];
}

int main(){
    int m=3,n=2;
    int a[m*n] = {1,2,3,4,5,6};
    int t[m*n];
    
    /*
    int *a,*t,m=3,n=2;
    printf("enter the value of m and n: \n");
    scanf("%d%d",&m,&n);
    a = (int *)malloc(m*n*sizeof(int));
    c = (int *)malloc(m*n*sizeof(int));
    printf("enter input matrix\n");
    for(int i=0; i<m*n; i++){
        scanf("%d",&a[i]);
    }
    */

    int *d_a, *d_t;
    int size = sizeof(int)*m*n;

    cudaMalloc((void **)&d_a,size);
    cudaMalloc((void **)&d_t,size);

    cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
    transpose<<<m,n>>>(d_a,d_t);
    cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
    printf("result vector is:\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            printf("%d\t",t[i*m+j]);
        }
        printf("\n");
    }
    cudaFree(d_a);
    cudaFree(d_t);
    return 0;
}