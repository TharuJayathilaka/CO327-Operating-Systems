#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[]){

	int source_file,target_file;

	//If the no of arguments are wrong
	if(argc!=3)
	{
		printf("No of input arguments are wrong. can add only source and target text files as an input\n");
		return 1;
	}

	//open source file if not print an error message
	source_file = open(argv[1],O_RDONLY);
	if(source_file == -1)
	{
		fprintf(stderr,"Error opening source file\n");
		return 1;
	}

	//create target file and open it if not print an error message
	target_file = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR );
	if(target_file == -1)
	{
		fprintf(stderr,"Error opening target file\n");
		return 1;
	}


	//Write the source file included things to target file

	size_t ret_1, ret_2;

	//read from source file and store it in a buffer if not print an error message
	char* buff = malloc(1000+1); /* size of a char is 1. This buffer holds the read back value. */
	ret_1 = read(source_file,buff,1000);
	if(ret_1 == -1)
	{
	fprintf(stderr,"Error reading the source file\n");
	return 1;
	}

	//write to target file using the buffer if not print an error message
	ret_2 = write(target_file,buff,ret_1);
	if(ret_2 == -1)
	{
	fprintf(stderr,"Error writing to the target file\n");
	return 1;
	}

	//clear everything and close the files.
	close(source_file);
	close(target_file);
	free(buff);

return 0;

}
