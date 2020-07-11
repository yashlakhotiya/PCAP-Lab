__kernel void repeatString(__global char*A, __global char*B, __global int*C)
{
	int i = get_global_id(0);
	int N = C[0];
	int lengthOfInputString = C[1];

	for(int j=0;j<lengthOfInputString;j++)
	{
		B[i*lengthOfInputString + j] = A[j];
	}
}