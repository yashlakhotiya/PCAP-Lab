__kernel void performTask(__global int*A, __global int *B)
{
	//Get the index
	int i = get_global_id(0);

	int n = A[i];


	    if(n<2)
	    {
	    	B[i] = n*n;
	    	return;
	    }

	    for(int j=2;j<=n/2;j++)
	    {
	        if(n%j==0)
	        {
	        	B[i] = n*n;
	        	return;
	        }
	    }
	    
	    B[i] = A[i];
	    return;






}