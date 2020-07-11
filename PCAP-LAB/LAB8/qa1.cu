#include <stdio.h>
#include <string.h>
__global__ void repeatString(char *inputString, int inputStringLength, char *outputString, int n)
{
	int i = threadIdx.x;

	for(int count = 0, j = i ; count < n ; count++, j+=inputStringLength)
	{
		outputString[j] = inputString[i];
	}
}

int main(void)
{
	char inputString[100], outputString[100];
	printf("Enter the string: "); gets(inputString);
	int inputStringLength = strlen(inputString);

	int n; printf("Enter N: "); scanf("%d",&n);

	char *d_inputString, *d_outputString;

	cudaMalloc((void**)&d_inputString, inputStringLength*sizeof(char));
	cudaMalloc((void**)&d_outputString,n*inputStringLength*sizeof(char));

	cudaMemcpy(d_inputString, inputString, inputStringLength*sizeof(char), cudaMemcpyHostToDevice);

	repeatString<<<1,inputStringLength>>>(d_inputString, inputStringLength, d_outputString, n);

	cudaMemcpy(outputString, d_outputString, n*inputStringLength*sizeof(char), cudaMemcpyDeviceToHost);
	outputString[n*inputStringLength] = '\0';
	printf("Output: %s\n", outputString);
	

	cudaFree(d_inputString);
	cudaFree(d_outputString);
	return 0;
}