__kernel void convertBinaryToDecimal(__global int*A, __global int *B)
{
	//Get the index
	int i = get_global_id(0);


	int factor = 1, answer = 0, temp;

	for(int j = A[i];j>0;j=j/10, factor = factor*2)
	{
		temp = j % 10;
		answer = answer + factor*temp;
	}

	B[i] = answer;





}