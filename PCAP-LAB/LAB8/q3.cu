%%cu
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__global__ void countWordFreq(char *str, char *word, int *freq, int len_word){
    int space = 0, start_index, end_index, len_word_device;
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
	len_word_device = end_index-start_index+1;
    
    if(len_word_device == len_word){
        int count = len_word;
        int i = 0;
        while(count != 0){
            if(str[start_index+i] != word[i]){
                break;
            }
            else{
                i++;
                count--;
            }
        }
        if(count == 0){
            atomicAdd(freq,1);
        }
    }
}

int main(){
    char str[1000] = "Quick A Quick Brown Fox Quick Quick Jumps Over The Lazy dog Quick";
    char word[1000] = "Quick";
    int num_words = 13;

    /*
	printf("enter number of words\n");
	scanf("%d",&num_words);

	printf("enter string\n");
	scanf("%s",str);

    printf("enter word\n");
	scanf("%s",word);
	*/

    int size_char = sizeof(char);
    int size_int = sizeof(int);
    int len_str = strlen(str);
    int len_word = strlen(word);

    int freq=0;

    char *d_a, *d_b;
    int *d_c;
    cudaMalloc((void **)&d_a,size_char*(strlen(str)+1));
    cudaMalloc((void**)&d_b,size_char*(strlen(word)+1));
    cudaMalloc((void**)&d_c,size_int*1);

    cudaMemcpy(d_a,str,size_char*(strlen(str)+1),cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,word,size_char*(strlen(word)+1),cudaMemcpyHostToDevice);

    countWordFreq<<<num_words,1>>>(d_a,d_b,d_c,len_word);

    cudaMemcpy(&freq,d_c,size_int*1,cudaMemcpyDeviceToHost);

    printf("frequency of '%s' in '%s' is %d\n",word,str,freq);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;

}