%%cu
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void reverseWord(char *str, char *rev_str){
	int space = 0, start_index, end_index, len_word;
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	char *ptr_start = str, *ptr_end, *ptr_space;
	ptr_space = ptr_start;
	while((*ptr_space) != ' ' && (*ptr_space) != '\0'){
		ptr_space++;
	}
	space++;

	while(space <= id){
		ptr_start = ptr_space + 1;
    ptr_space = ptr_start;
		while((*ptr_space) != ' ' && (*ptr_space) != '\0'){
      ptr_space++;
    }
		space++;
	}

	ptr_end = ptr_space - 1;
  	start_index = ptr_start - str;
	end_index = ptr_end - str;
	len_word = end_index-start_index+1;

	for(int i=start_index,j=end_index; i<=end_index,j>=start_index; i++,j--){
		rev_str[i] = str[j];
	}
	if(id == gridDim.x-1){
		rev_str[end_index+1] = '\0';
	}
	else{
		rev_str[end_index+1] = ' ';
	}
	printf("id: %d, st_ind: %d, end_ind: %d, len: %d\n",id,start_index,end_index,len_word);

}
//string should end with a space
int main(){
	char str[1000] = "A Quick Brown Fox Jumps Over The Lazy dog",	rev_str[1000];
	int num_words = 9;
	printf("%s\n",str);
	for(int i=0;i<strlen(str); i++){
		printf("%d",i%10);
	}
	printf("\n");
	/*
	printf("enter number of words\n");
	scanf("%d",&num_words);

	printf("enter string\n");
	scanf("%s",str);
	*/
  	int size = sizeof(char);
	int len = strlen(str)+1;

	char *d_a, *d_b;

	cudaMalloc((void **)&d_a,size*len);
	cudaMalloc((void **)&d_b,size*len);

	cudaMemcpy(d_a,str,size*len,cudaMemcpyHostToDevice);

	reverseWord<<<num_words,1>>>(d_a,d_b);

	cudaMemcpy(rev_str,d_b,size*len,cudaMemcpyDeviceToHost);

	printf("reversed string is: %s\n",rev_str);

	cudaFree(d_a);
	cudaFree(d_b);
}