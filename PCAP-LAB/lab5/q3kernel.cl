__kernel void swapAlternateElements(__global int*A)
{
	//Get the index
	int i = get_global_id(0);
	i = i*2;

	int temp = A[i];
	A[i] = A[i+1];
	A[i+1] = temp;
}