__kernel void findOnesComplement(__global int*A, __global int *B)
{
	//Get the index
	int i = get_global_id(0);


	int factor = 1, answer = 0, temp;

	for(int j = A[i];j>0;j=j/10, factor = factor*10)
	{
		temp = j % 10;

		if(temp==0)
		{
			answer += factor*1;
		}
		else//temp==1
		{
			answer += factor*0;
		}
	}

	B[i] = answer;





}