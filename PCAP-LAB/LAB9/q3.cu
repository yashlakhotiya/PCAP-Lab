%%cu
#include <stdio.h>
#include <stdlib.h>


__global__ void row_sum(int *a, int *b, int *res){
    int i = blockIdx.x;
    int n = blockDim.x;//number of columns
    for(int k=0; k<n; k++){
        res[i*n+k] = a[i*n+k] + b[i*n+k];
    }
}

__global__ void column_sum(int *a, int *b, int *res){
    int j = threadIdx.x;
    int m = gridDim.x;//number of rows
    for(int k=0; k<m; k++){
        res[j*m+k] = a[j*m+k] + b[j*m+k];
    }
}

__global__ void cell_sum(int *a, int *b, int *res){
    int i = blockIdx.x;
    int j = threadIdx.x;
    int m = gridDim.x;
    int n = blockDim.x;

    res[i*n+j] = a[i*n+j] + b[i*n+j];
}

int main(){
    int m=3,n=2;
    int a[m*n] = {1,2,3,4,5,6};
    int b[m*n] = {1,2,3,4,5,6};
    int res[m*n];
    
    /*
    int *a,*b,*res,m=3,n=2;
    printf("enter the value of m and n: \n");
    scanf("%d%d",&m,&n);
    a = (int *)malloc(m*n*sizeof(int));
    c = (int *)malloc(m*n*sizeof(int));
    printf("enter input matrix\n");
    for(int i=0; i<m*n; i++){
        scanf("%d",&a[i]);
    }
    */

    int *d_a, *d_b, *d_res;
    int size = sizeof(int)*m*n;

    cudaMalloc((void **)&d_a,size);
    cudaMalloc((void **)&d_b,size);
    cudaMalloc((void **)&d_res,size);

    cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);
    row_sum<<<m,n>>>(d_a,d_b,d_res);
    cudaMemcpy(res,d_res,size,cudaMemcpyDeviceToHost);
    printf("row sum : result vector is:\n");
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            printf("%d\t",res[i*n+j]);
        }
        printf("\n");
    }

    column_sum<<<m,n>>>(d_a,d_b,d_res);
    cudaMemcpy(res,d_res,size,cudaMemcpyDeviceToHost);
    printf("column sum : result vector is:\n");
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            printf("%d\t",res[i*n+j]);
        }
        printf("\n");
    }

    cell_sum<<<m,n>>>(d_a,d_b,d_res);
    cudaMemcpy(res,d_res,size,cudaMemcpyDeviceToHost);
    printf("cell sum : result vector is:\n");
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            printf("%d\t",res[i*n+j]);
        }
        printf("\n");
    }
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_res);
    return 0;
}