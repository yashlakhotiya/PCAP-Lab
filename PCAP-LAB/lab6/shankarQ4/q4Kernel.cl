__kernel void hello_print(__global char *A,__global char *B,__global int *l)
{
	int k=0,s=0,j=0,len;
	int i = get_global_id(0);

	for(j=0;j<i;j++)
	{
		s=s+l[j]+1;
	}
	len=l[i];
	for(k=0;k<len;k++)
	{
		B[s+len-k-1]=A[s+k];
	}
	B[s+len]=' ';
}