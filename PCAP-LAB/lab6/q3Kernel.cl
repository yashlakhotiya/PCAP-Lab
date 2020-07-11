__kernel void odd(__global int *a)
{
	int id = get_global_id(0);
	int n = get_global_size(0), t;

	if(id%2==0 && (id+1)!=n)
	{
		if(a[id]>a[id+1])
		{
			t = a[id]; a[id] = a[id+1]; a[id+1] = t;
		}
	}
}


__kernel void even(__global int *a)
{
	int id = get_global_id(0);
	int n = get_global_size(0), t;

	if(id%2!=0 && (id+1)!=n)
	{
		if(a[id]>a[id+1])
		{
			t = a[id]; a[id] = a[id+1]; a[id+1] = t;
		}
	}
}