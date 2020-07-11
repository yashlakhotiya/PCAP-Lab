%%cu
#include <stdio.h>
#include <stdlib.h>


__global__ void row_power(int *a, int *t){
    int n = threadIdx.x;
    int m = blockIdx.x;
    int size = blockDim.x;//number of columns
    int size1 = gridDim.x;//number of rows
    t[m*size+n] = pow(a[m*size+n],m+1);
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
    row_power<<<m,n>>>(d_a,d_t);
    cudaMemcpy(t,d_t,size,cudaMemcpyDeviceToHost);
    printf("result vector is:\n");
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            printf("%d\t",t[i*n+j]);
        }
        printf("\n");
    }
    cudaFree(d_a);
    cudaFree(d_t);
    return 0;
}