#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdio.h>
#include <string.h>


int main() 
{ 
    int fd1,fd2; 
	
	char buff[1000]; 
	printf("Enter the text which needs to be capitalized:");
    fgets(buff,1000,stdin);
	
	char* fifo1 = "/tmp/fifo1"; 
	char* fifo2 = "/tmp/fifo2";

    mkfifo(fifo1,0666);
	mkfifo(fifo2, 0666);
	
	//write to pipe the text which need to capitalized
	fd1 = open(fifo1, O_WRONLY);
    write(fd1, buff,strlen(buff));
    close(fd1);

	//read from the pipe
    fd2 = open(fifo2, O_RDONLY);
    read(fd2, buff, 1000);
    printf("Capitalized text: %s\n", buff);
	
    close(fd2);
    unlink(fifo1);
    unlink(fifo2); 

    return 0; 
}

