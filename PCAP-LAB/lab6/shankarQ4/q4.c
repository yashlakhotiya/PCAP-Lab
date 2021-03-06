#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define MAX_SOURCE_SIZE (0x100000)

int main(void)
{
	time_t start,end;
	start = clock();

	char tempstr[200];

	int i,l,j;
	printf("Input string: ");
	// scanf("%s",tempstr);
	gets(tempstr);
	i=0,j=0;
	int lens[100];
	// printf("%s",tempstr);
	while(tempstr[i]!='\0')
	{
		l=0;
		while(tempstr[i]!=32 && tempstr[i]!='\0')
		{
			l++;
			i++;
		}
		lens[j]=l;
		j++;
		i++;
	}
	int n=j;
	/*for(int d=0;d<n;d++)
		printf("%d ",lens[d]);
	*/
	int len=strlen(tempstr);
	// printf("%d\n",len);
	// len++;

	char * str = (char*)malloc(sizeof(char)*len);
	strcpy(str,tempstr);

	FILE* fp;
	char* source_str;
	size_t source_size;

	fp = fopen("q4Kernel.cl","r");

	if(!fp)
	{
		fprintf(stderr,"Failed to load kernel");
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

	ret = clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_ALL,1,&device_id,&ret_num_devices);
	// printf("%d\n",ret);

	cl_context context = clCreateContext(NULL,1,&device_id,NULL,NULL,&ret);

	cl_command_queue command_queue = clCreateCommandQueue(context,device_id,CL_QUEUE_PROFILING_ENABLE,&ret);

	cl_mem a_mem_obj = clCreateBuffer(context,CL_MEM_READ_ONLY,len*sizeof(char),NULL,&ret);
	cl_mem l_mem_obj = clCreateBuffer(context,CL_MEM_READ_ONLY,(n+1)*sizeof(int),NULL,&ret);
	cl_mem b_mem_obj = clCreateBuffer(context,CL_MEM_WRITE_ONLY,len*sizeof(char),NULL,&ret);

	ret = clEnqueueWriteBuffer(command_queue,a_mem_obj,CL_TRUE,0,len*sizeof(char),str,0,NULL,NULL);
	// printf("%d\n",ret);
	ret = clEnqueueWriteBuffer(command_queue,l_mem_obj,CL_TRUE,0,(n+1)*sizeof(int),lens,0,NULL,NULL);
	// printf("%d\n",ret);
	cl_program program = clCreateProgramWithSource(context,1,(const char**)&source_str,(const size_t*)&source_size,&ret);
	// printf("%d\n",ret);
	ret = clBuildProgram(program,1,&device_id,NULL,NULL,NULL);
	// printf("%d\n",ret);
	cl_kernel kernel=clCreateKernel(program,"hello_print",&ret);

	ret = clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&a_mem_obj);
	ret = clSetKernelArg(kernel,1,sizeof(cl_mem),(void*)&b_mem_obj);
	ret = clSetKernelArg(kernel,2,sizeof(cl_mem),(void*)&l_mem_obj);
	// printf("%d\n",ret);
	size_t global_item_size = n;
	size_t local_item_size = 1;

	cl_event event;
	ret = clEnqueueNDRangeKernel(command_queue,kernel,1,NULL,&global_item_size,&local_item_size,0,NULL,&event);
	// printf("%d\n",ret);
	time_t stime = clock();

	ret = clFinish(command_queue);
	// printf("%d\n",ret);
	cl_ulong time_start,time_end;
	double total_time;

	clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_START,sizeof(time_start),&time_start,NULL);

	clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_END,sizeof(time_end),&time_end,NULL);

	total_time = (double)(time_end - time_start);

	char* strres = (char*)malloc(sizeof(char)*len);

	ret = clEnqueueReadBuffer(command_queue,b_mem_obj,CL_TRUE,0,len*sizeof(char),strres,0,NULL,NULL);
	// printf("\nDone");
	// printf("%d\n",ret);
	strres[len]='\0';
	printf("Output string: %s\n",strres);

	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(b_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	/*end = clock();

	printf("Time(Kernel):%0.3f\n",total_time/1000000);
	printf("Time(Program):%0.3f",(end-start)/(double)CLOCKS_PER_SEC);
*/
	free(str);
	free(strres);
	//getch();
	return 0;
}