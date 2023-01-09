#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc , char *argv[] )
{
	int out;
	size_t size = 1000;
	
	//If the no of arguments are wrong
	if(argCount!=2)
	{
        printf("No of input arguments are wrong. can add only one text file as an input\n");
        return 0;
    }
	
	//reads a text file

    //the name of the text file is given as an argument	
	out = open(argv[1], O_RDONLY);
	if(out == -1)
	{
		/* the exact error could be found out by looking at the variable errno. We do not cover it here */
		fprintf(stderr,"Couldn't open the filefor writing\n");
		return 1;
	} 

	char* buffer = malloc(size+1); /* size of a char is 1. This buffer holds the read back value. */
	size_t ret = read(out,buffer,size);

	if(ret == -1)
	{
		fprintf(stderr,"Error reading from file\n");
		return 1;
	}
	
	//writes to the standard output	
	
	buffer[ret] = '\0'; /* we have to null terminate the string ourselves. */
	printf("The string read back is: %s\n",buffer); //writes to the standard output	
	/* In case there was something already written in the file, the text read back might not be the same as what was written */

	// clear everything
	free(buffer);
	ret = close(out);

	if(ret == -1)
	{
		fprintf(stderr,"Error closing the file after reading.\n");
		return 1;
	}

	return 0;
}
