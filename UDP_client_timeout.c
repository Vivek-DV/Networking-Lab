#include <stdio.h> 
#include <strings.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include<netinet/in.h> 
#include<unistd.h> 
#include<stdlib.h> 
#include <errno.h>

#define PORT 5000 
#define MAXLINE 1000 
#define RETERR -1

struct frame{
	char ch;
	int frame_no;
};

// Driver code 
int main() 
{ 
	char buffer[100]; 
	char *message = "Hello Server"; 
	int sockfd, n; 
	struct sockaddr_in servaddr; 
	
	// clear servaddr 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_family = AF_INET; 
	
	// create datagram socket 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	
	//Define the timeout value
	struct timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	//Set timeout for the socket
	int retVal = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	if(retVal < 0)
	{
		printf("\nError in setsockopt");
		close(sockfd);
		exit(1);
	}
	// connect to server 
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
	{ 
		printf("\n Error : Connect Failed \n"); 
		exit(0); 
	} 

	// request to send datagram 
	// no need to specify server address in sendto 
	// connect stores the peers IP and port 
	
	struct frame newFrame;
	newFrame.ch = 'u';
	newFrame.frameNo = 0;

	//sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	sendto(sockfd, &newFrame, sizeof(struct frame), 0, (struct sockaddr*)NULL, sizeof(servaddr)); 
	
	// waiting for response 
	int bytesReceived= recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL); 
	if(bytesReceived == -1  && errno == EWOULDBLOCK){
		printf("\nReceive timed out...\n");
	}
	else{
		if(bytesReceived)
			puts(buffer);
		else
			printf("\nZero bytes received");
	}	

	// close the descriptor 
	close(sockfd); 
} 

