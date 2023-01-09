#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <stdlib.h> 
#include <pthread.h> 

void* handle_client(void*); 

int main() 
{
	int listenfd; 
	int* connfd; 
	struct sockaddr_in servaddr,cliaddr; 
	socklen_t clilen; 
	listenfd = socket(AF_INET,SOCK_STREAM,0); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(32000); 
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)); 
	listen(listenfd,5); 
	clilen = sizeof(cliaddr); 
	while(1) 
	{ 
		connfd = malloc(sizeof(int)); 
		*connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen); 
		/* now create a new thread, pass it the socket and run the thread. */ 
		pthread_t client_thread;
		if (pthread_create(&client_thread, NULL, handle_client, connfd)) //create a new thread, pass it the socket and run the thread
		{
			printf("Error creating the thread");
		}
	} 
}


void* handle_client(void* connfd) 
{
	/*read a string sent by the client, 
	print it and then send the string
	“Hello from the server” to the client*/
	
	int readfd = *((int *)connfd);	
	char buff[1000];
   	read(readfd , buff, 1000); //read a string sent by the client
   	printf("Client send : %s\n",buff);// print the string sent by the client
	strcpy(buff,"Hello from the server\n");
    send(readfd , buff , strlen(buff) , 0 ); //send the string “Hello from the server” to the client
    close(readfd);
	free(connfd); 
	return NULL;
} 