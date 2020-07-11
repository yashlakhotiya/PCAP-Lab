#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void error_handler(int error_code){
	if(error_code != MPI_SUCCESS){
		char error_string[BUFSIZ];
		int length_of_string, error_class;
		MPI_Error_class(error_code,&error_class);
		MPI_Error_string(error_class,error_string,&length_of_string);
		fprintf(stderr, "%s %d\n",error_string,length_of_string);
	}
}

int main(int argc, char *argv[]){
	int rank,size,err;
	int c = 2;
	err = MPI_Init(&argc,&argv);
	MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
	MPI_Comm_rank(c,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int n;
	n = 1;
	MPI_Finalize();
	return 0;
}