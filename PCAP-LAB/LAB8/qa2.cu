#include <stdio.h>
#include <string.h>
__global__ void repeatEachCharInxexNumberOfTimes(char *inputString, char *outputString)
{
	int index = threadIdx.x;
	int startIndex = (index*(index+1))/2;
	int numberOfTimes = index+1;
	char toBeRepeated = inputString[index];

	for (int i = 0; i < numberOfTimes; ++i)
	{
		outputString[startIndex + i] = toBeRepeated;
	}
}

int main(void)
{
	char inputString[100], outputString[100];
	printf("Enter the string: "); gets(inputString);
	int inputStringLength = strlen(inputString);
	int outputStringLength = ((inputStringLength*(inputStringLength+1)/2)*sizeof(char));

	char *d_inputString, *d_outputString;
	// int *d_inputStringLength;

	cudaMalloc((void**)&d_inputString, inputStringLength*sizeof(char));
	// cudaMalloc((void**)&d_inputStringLength,1*sizeof(int));
	cudaMalloc((void**)&d_outputString, outputStringLength*sizeof(char));

	cudaMemcpy(d_inputString, inputString, inputStringLength*sizeof(char), cudaMemcpyHostToDevice);
	// cudaMemcpy(d_inputStringLength, &inputStringLength, 1*sizeof(int), cudaMemcpyHostToDevice);

	repeatEachCharInxexNumberOfTimes<<<1,inputStringLength>>>(d_inputString, d_outputString);

	cudaMemcpy(outputString, d_outputString, outputStringLength*sizeof(char), cudaMemcpyDeviceToHost);
	outputString[outputStringLength] = '\0';
	printf("Output: %s\n", outputString);
	

	cudaFree(d_inputString);
	// cudaFree(d_inputStringLength);
	cudaFree(d_outputString);
	return 0;
}