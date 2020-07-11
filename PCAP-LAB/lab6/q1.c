#include<stdio.h>
#include<CL/cl.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
//#include<conio.h>

#define MAX_SOURCE_SIZE 0x10000 


int main()
{
	time_t start, end;
	start = clock();
	char tempstr[50]; int i;


	printf("Enter string: "); //fgets(tempstr, 50, stdin);
	scanf("%s",tempstr);

	int lengthOfInputString = strlen(tempstr);
	int len = strlen(tempstr); len++;


	int N; printf("Enter N:"); scanf("%d",&N);

	char *str = (char*) malloc(sizeof(char)*len);
	strcpy(str, tempstr);

	FILE *fp = fopen("q1Kernel.cl","r");
	char *source_str; size_t source_size;

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

	cl_mem s_mem_obj = clCreateBuffer(context,CL_MEM_READ_ONLY,len * sizeof(char), NULL, &ret);
	cl_mem t_mem_obj = clCreateBuffer(context,CL_MEM_WRITE_ONLY,len * N * sizeof(char), NULL, &ret);
	cl_mem myNvalueObj = clCreateBuffer(context, CL_MEM_READ_ONLY, 2 * sizeof(int), NULL, &ret);//first is N and second is lengthOfInputString


	//Copy the lists A and B to buffer
	int arr[2]; arr[0] = N; arr[1] = lengthOfInputString;
	ret = clEnqueueWriteBuffer(command_queue,s_mem_obj,CL_TRUE,0,len * sizeof(char),str,0,NULL,NULL);
	ret = clEnqueueWriteBuffer(command_queue,myNvalueObj,CL_TRUE,0,2 * sizeof(int),arr,0,NULL,NULL);

	//Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str,(const size_t *)&source_size,&ret);

	//Build the program
	ret = clBuildProgram(program, 1,&device_id, NULL,NULL,NULL);

	//create the openCL kernel obj
	cl_kernel kernel = clCreateKernel(program, "repeatString",&ret);

	//Set the arguments of the kernel
	ret = clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&s_mem_obj);
	ret = clSetKernelArg(kernel,1,sizeof(cl_mem),(void*)&t_mem_obj);
	ret = clSetKernelArg(kernel,2,sizeof(cl_mem),(void*)&myNvalueObj);

	//Execute the openCL kernel on the array
	size_t global_item_size = N;
	size_t local_item_size = 1;




	//Execute the kernel on the device
	cl_event event;
	ret = clEnqueueNDRangeKernel(command_queue, kernel,1,NULL,&global_item_size, &local_item_size,0,NULL,&event);
	
	
	time_t stime = clock();

	ret = clFinish(command_queue);

	cl_ulong time_start, time_end;
	double total_time;

	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = (double)(time_end - time_start);



	char *strres = (char *)malloc(sizeof(char)*lengthOfInputString*N);
	ret = clEnqueueReadBuffer(command_queue, t_mem_obj, CL_TRUE, 0, N * lengthOfInputString * sizeof(char), strres, 0, NULL, NULL);

	printf("Done\n");
	strres[N*lengthOfInputString] = '\0';

	printf("Resultant toggled string: %s\n", strres);
	getchar();

	
	//CleanUp
	//ret = clFlush(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(s_mem_obj);
	ret = clReleaseMemObject(t_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	end = clock();

	printf("Time taken to execute the kernel in ms: %0.3f\n", total_time/1000000);
	printf("Time taken to execute the whole program in sec: %0.3f\n", (end-start)/(double)CLOCKS_PER_SEC);

	free(str);
	free(strres);
	getchar();
	return 0;
}