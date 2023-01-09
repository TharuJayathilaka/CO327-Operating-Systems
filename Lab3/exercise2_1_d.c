#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define READ_END 0
#define WRITE_END 1

//const char banner [] = "hello there\n";

int main()
{
	//Define the two pipes; parent to child pipe and child to parent pipe
	int pipe_pc[2],pipe_cp[2];
	pid_t pid;
	
	// see the creation of two pipes otherwise print an error message
	if(pipe(pipe_pc)) 
	{
		perror("Pipe pc creation Failed");
		return -1;
	}
	if(pipe(pipe_cp)) 
	{
		perror("Pipe cp creation Failed");
		return -1;
	}
	
	
	//Using fork create the child process and if not print an eeror message.
	pid = fork();
	if(pid < 0) 
	{
		perror("Couldn't create child process");
		return -1;
	}
	
	
	//-----------PARENT---------------------------------------
	if(pid > 0) 
	{ 
		char buff2[1000];
		close(pipe_pc[READ_END]);
		
		printf("PARENT: Enter text: ");
		fgets(buff2,1000,stdin);

		printf("PARENT: Read from user: %s\n",buff2);

		// send to the child the string to get capitalised
		close(pipe_pc[READ_END]);
		write(pipe_pc[WRITE_END],buff2,strlen(buff2));
		// get back the capitalised value through the pipe_cp
		close(pipe_cp[WRITE_END]);
		read(pipe_cp[READ_END],buff2,1000);

		printf("PARENT: Read from child: %s\n",buff2);
		
	}
	
	
	//---------------CHILD-----------------------------------
	if(pid == 0) 
	{ 
		char buff1[1000];
		close(pipe_pc[WRITE_END]);

		int count = read(pipe_pc[READ_END],buff1,1000);
		
		//capitalise the input that parent sent
		for(int i=0;i<count;i++)
		{
			buff1[i]=(char)toupper(buff1[i]);
		}
		buff1[count] = '\0';
		
		// send back the capitalised value through the pipe_cp
		close(pipe_cp[READ_END]);
		write(pipe_cp[WRITE_END],buff1,count);

		exit(EXIT_SUCCESS);
	}
	return 0;
}
