__kernel void vector_add(__global int*A, __global int *B)
{
	//Get the index
	int i = get_global_id(0);
	//Do the op
	int j,t,f;
	f = 0;
	t = 1;
	for(j=A[i];j>0;j=j/8)
	{
		f = f + (j%8)*t;
		t = t*10;
	}
	B[i] = f;
}