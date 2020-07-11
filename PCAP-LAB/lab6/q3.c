#include<stdio.h>
#include<CL/cl.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
//#include<conio.h>

#define MAX_SOURCE_SIZE 0x10000 


int main()
{
	int n; printf("Enter n: "); scanf("%d",&n);

	printf("Enter the elements: ");
	int arr[n];
	for(int i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}

	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("q3Kernel.cl","r");

	if(!fp)
	{
		fprintf(stderr,"Failed to load kernel.\n");
		getchar();
		exit(1);
	}

	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str,1,MAX_SOURCE_SIZE,fp);
	fclose(fp);
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret = clGetPlatformIDs(1,&platform_id,&ret_num_platforms);
	ret = clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_CPU,1,&device_id,&ret_num_devices);
	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&ret);
	cl_command_queue command_queue = clCreateCommandQueue(context,device_id,CL_QUEUE_PROFILING_ENABLE,&ret);

	cl_mem s_mem_obj = clCreateBuffer(context,CL_MEM_READ_ONLY, n * sizeof(int), NULL, &ret);

	ret = clEnqueueWriteBuffer(command_queue,s_mem_obj,CL_TRUE,0,n * sizeof(int),arr,0,NULL,NULL);

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size,&ret);
	ret = clBuildProgram(program, 1,&device_id, NULL,NULL,NULL);
	cl_kernel kernel1 = clCreateKernel(program, "odd", &ret);
	cl_kernel kernel2 = clCreateKernel(program, "even", &ret);

	ret = clSetKernelArg(kernel1,0,sizeof(cl_mem),(void*)&s_mem_obj);

	ret = clSetKernelArg(kernel2,0,sizeof(cl_mem),(void*)&s_mem_obj);

	size_t global_item_size = n;
	size_t local_item_size = 1;

	cl_event event;


	for(int i=0; i<( (n/2) +1);i++)
	{
		ret = clEnqueueNDRangeKernel(command_queue, kernel1,1,NULL,&global_item_size, &local_item_size,0,NULL,&event);
		ret = clFinish(command_queue);
		ret = clEnqueueNDRangeKernel(command_queue, kernel2,1,NULL,&global_item_size, &local_item_size,0,NULL,&event);
		ret = clFinish(command_queue);
	}


	int *strres = (int*)malloc(n * sizeof(int));
	ret = clEnqueueReadBuffer(command_queue, s_mem_obj, CL_TRUE, 0, n * sizeof(int), strres, 0, NULL, NULL);

	printf("Sorted array: ");
	for(int i=0;i<n;i++)
		printf("%d ", strres[i]);
	printf("\n");
	getchar();

	//CleanUp
	//ret = clFlush(command_queue);
	ret = clReleaseKernel(kernel1);
	ret = clReleaseKernel(kernel2);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(s_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);


	getchar();
	return 0;
}