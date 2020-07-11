#include<stdio.h>
#include<CL/cl.h>
#include<stdlib.h>

//Max source size of the kernel string
#define MAX_SOURCE_SIZE 0x100000 

int main(void)
{
	//create the two input vectors
	int i;
	int LIST_SIZE;
	printf("How many elements:");
	scanf("%d",&LIST_SIZE);

	int *A = (int*)malloc(sizeof(int)*LIST_SIZE);

	printf("Enter the numbers :\n");
	//Initialize the input vectors
	for(i=0;i<LIST_SIZE;i++)
	scanf("%d",&A[i]);

	//Load the kernel source code into the array source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("qa2kernel.cl","r");

	if(!fp)
	{
	fprintf(stderr,"Failed to load kernel.\n");
	getchar();
	exit(1);
	}

	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str,1,MAX_SOURCE_SIZE,fp);

	fclose(fp);

	//Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	cl_int ret = clGetPlatformIDs(1,&platform_id,&ret_num_platforms);
	ret = clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_CPU,1,&device_id,&ret_num_devices);

	//Craete an OpenCL context

	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&ret);

	//Create a command queue
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context,device_id,NULL,&ret);

	//Create a memory buffers on the device for each vector A and B
	
	cl_mem a_mem_obj = clCreateBuffer(context,CL_MEM_READ_ONLY,LIST_SIZE * sizeof(int), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context,CL_MEM_READ_ONLY,LIST_SIZE * sizeof(int), NULL, &ret);

	//Copy the lists A and B to buffer
	ret = clEnqueueWriteBuffer(command_queue,a_mem_obj,CL_TRUE,0,LIST_SIZE * sizeof(int),A,0,NULL,NULL);

	//Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size,&ret);

	//Build the program
	ret = clBuildProgram(program, 1,&device_id, NULL,NULL,NULL);

	//create the openCL kernel obj
	cl_kernel kernel = clCreateKernel(program, "performTask",&ret);

	//Set the arguments of the kernel
	ret = clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&a_mem_obj);
	ret = clSetKernelArg(kernel,1,sizeof(cl_mem),(void*)&b_mem_obj);

	//Execute the openCL kernel on the array
	size_t global_item_size = LIST_SIZE;
	size_t local_item_size = 1;

	//Execute the kernel on the device
	cl_event event;
	ret = clEnqueueNDRangeKernel(command_queue, kernel,1,NULL,&global_item_size, &local_item_size,0,NULL,NULL);

	ret = clFinish(command_queue);

	//Read the memory buffer B on the device to the local variable B

	int *B = (int *)malloc(sizeof(int)*LIST_SIZE);
	ret = clEnqueueReadBuffer(command_queue, b_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(int), B, 0, NULL, NULL);

	//Display the result on the screen
	printf("Output array:\n");
	for(i=0;i<LIST_SIZE;i++)
	{
		printf("%d ", B[i]);
	}
	printf("\n");

	//CleanUp
	ret = clFlush(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(b_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(A);
	free(B);
	getchar();
	return 0;
}