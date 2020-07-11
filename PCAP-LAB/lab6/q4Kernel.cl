#pragma OPENCL EXTENSION cl_intel_printf : enable 

__kernel void reverseWords(__global char*string, __global int*startIndices, __global int*finishIndices)
{
	int id = get_global_id(0);
	int start = startIndices[id];
	int end = finishIndices[id];
	int lengthOfThisWord = end - start + 1;
	char temp;

	for(int i=start;i<=end;i++)
	{
		printf("%c",string[i]);
	} printf("\n");

	while(start<end)
	{
		temp = string[start]; string[start] = string[end]; string[end] = temp;
		start++;
		end--;
	}
}