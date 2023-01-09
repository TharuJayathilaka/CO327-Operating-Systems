#include <fcntl.h> 
#include <stdio.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h>
#include <string.h>


#define MAX_SIZE 1024 

int main() 
{ 
    int fd1, fd2; 
	char* fifo1 = "/tmp/fifo1"; 
	char* fifo2 = "/tmp/fifo2";

	char *tr_args[] = {"tr", "[:lower:]", "[:upper:]", NULL};
	
	mkfifo(fifo1,0666);
	mkfifo(fifo2, 0666);
	
	fd1 = open(fifo1, O_RDONLY);
    fd2 =open(fifo2,O_WRONLY);
	
	dup2(fd1,fileno(stdin));
    dup2(fd2,fileno(stdout));
	
	execvp(tr_args[0],tr_args);

    return 0;
}