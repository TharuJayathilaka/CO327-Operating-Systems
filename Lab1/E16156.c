/*

CO327: Operating Systems 
Simple Shell

Name	: Jayathilaka H.A.D.T.T.
E-No	: E/16/156	

Implement a simple shell program that would; 
	• prompt the user to enter a command 
	• process the command (make sure to skip white spaces etc)
	• find out if the command is there in /bin 
	• if so, execute it as a child process 
	• wait for the next command
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h>
#include<unistd.h>

#define COMMAND_LEN 1024

int main(){
	
	// Define the all the variables which are needed to implement the simple shell
    char command[COMMAND_LEN];  
    char* user_input[100];        
	int word_count;              
    char* bin_path= "/bin/";   
    char programme_path[20];    
	char *token; 
	int status;	

	while(1){

		printf("My shell$ ");   //print the shell name

		strcpy(programme_path, bin_path);      // By copying /bin/ to file path make the path ready

		// By getting the command make the destination of the path
		if(!fgets(command, COMMAND_LEN, stdin)){  //get command and put it as the command
			break;                 
		}
	
		if (command[strlen(command) - 1] == '\n'){  // remove \n because fgets() keeps the '\n' when we press enter
			command[strlen(command) - 1] = '\0';
		}
		if(strcmp(command, "exit")==0){  //check if command is 'exit' or not if exit break
			break;
		}
		
		word_count=0;
		token = strtok(command," ");  // break command into a series of tokens using the space.
		
		if(token==NULL){  //Remove white spaces
			continue;    // if user press enter key nothing should happen except going for another line
		}
		
		while(token!=NULL && word_count < 100){
			user_input[word_count]=token;    // store all the wors seperated from the command to a array  
			token = strtok(NULL," ");
			word_count++;
		}
		user_input[word_count]=NULL;  //Last value shold be NULL to run execvp

		for(int i=0; i<word_count; i++){
			printf("%s\n", user_input[i]);      //print command for confarmation
		}
		
		strcat(programme_path, user_input[0]); //add program to path set the path destination

		for(int i=0; i<strlen(programme_path); i++){  // remove new line from the path as well
			if(programme_path[i]=='\n'){      
				programme_path[i]='\0';
			}
		}
		
		int pid= fork();     

/* Use a unique id for every process to keep track of all processes. And for that, fork() doesn’t take any parameter and return an int value as following:

	Zero: if it is the child process (the process created).
	Positive value: if it is the parent process.
	Negative value: if an error occurred.
*/
		if(pid==0){               // Newly spawned child Process. This will be taken over by "ls -l"
			execvp(programme_path,user_input);   // When execvp() is executed, the program file given by the first argument will be loaded into the caller's address space 
			                                     // and over-write the program there.
			fprintf(stderr, "ls -l has taken control of this child process. This won't execute unless it terminates abnormally!   ");

		}else if(pid>0){    //This is run by the parent.  Wait for the child to terminate.
			do {
				waitpid(pid, &status, WUNTRACED);   // waitpid function wait till child process ends
			} while(!WIFEXITED(status) && !WIFSIGNALED(status));
			printf("Child exited\n");
			
		}else if(pid<0){                    // Negative value means there is an error
			perror("Fork function didn't work");
		}else{                              // process id cannot be a null value
			perror("Command is not in the bin");
		}

	}
} 