//not working
#include<stdio.h>
#include<CL/cl.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
//#include<conio.h>

#define MAX_SOURCE_SIZE 0x10000 


int main()
{
	int numberOfWords; printf("Enter numberOfWords: "); scanf("%d",&numberOfWords);

	printf("Enter the string: "); char arr[100]; getchar(); gets(arr); int lengthOfInputString = strlen(arr);

	int startIndices[100], finishIndices[100];
	startIndices[0] = 0;
	finishIndices[numberOfWords-1] = lengthOfInputString - 1;
	int index = 0;

	for(int i=0;i<strlen(arr);i++)
	{
		if(arr[i]==' ')
		{
			finishIndices[index] = i-1;
			startIndices[++index] = i+1;
		}
	}

/*	if(index+1==numberOfWords)
	{
		printf("yahoo\n");
	}*/

	


	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("q4Kernel.cl","r");

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

	cl_mem string_obj = clCreateBuffer(context,CL_MEM_READ_WRITE, lengthOfInputString * sizeof(char), NULL, &ret);
	cl_mem startIndices_obj = clCreateBuffer(context,CL_MEM_READ_ONLY, numberOfWords * sizeof(int), NULL, &ret);
	cl_mem finishIndices_obj = clCreateBuffer(context,CL_MEM_READ_ONLY, numberOfWords * sizeof(int), NULL, &ret);

	ret = clEnqueueWriteBuffer(command_queue,string_obj,CL_TRUE,0,lengthOfInputString * sizeof(char),arr,0,NULL,NULL);
	ret = clEnqueueWriteBuffer(command_queue,startIndices_obj,CL_TRUE,0,numberOfWords * sizeof(int),startIndices,0,NULL,NULL);
	ret = clEnqueueWriteBuffer(command_queue,finishIndices_obj,CL_TRUE,0,numberOfWords * sizeof(int),finishIndices,0,NULL,NULL);

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size,&ret);
	ret = clBuildProgram(program, 1,&device_id, NULL,NULL,NULL);
	cl_kernel kernel = clCreateKernel(program, "reverseWords", &ret);

	ret = clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&string_obj);
	ret = clSetKernelArg(kernel,1,sizeof(cl_mem),(void*)&startIndices);
	ret = clSetKernelArg(kernel,2,sizeof(cl_mem),(void*)&finishIndices);

	size_t global_item_size = numberOfWords;
	size_t local_item_size = 1;

	cl_event event;

	ret = clEnqueueNDRangeKernel(command_queue, kernel,1,NULL,&global_item_size, &local_item_size,0,NULL,&event);
	ret = clFinish(command_queue);

	char *strres = (char*)malloc(lengthOfInputString * sizeof(char));
	ret = clEnqueueReadBuffer(command_queue, string_obj, CL_TRUE, 0, lengthOfInputString * sizeof(char), strres, 0, NULL, NULL);
	strres[lengthOfInputString] = '\0';

	printf("Resultant string: "); puts(strres);
	printf("\n");
	getchar();

	//CleanUp
	//ret = clFlush(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);


	//getchar();
	return 0;
}


/*
4
my name is pranav


*/