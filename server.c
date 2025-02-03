#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define port 9001


int main()
{
	//create the socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		perror("ERROR in opening socket");
	}
	
	//assign address to server socket
	struct sockaddr_in server_addr, client_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	//bind socket to port number
	bind(server_socket,(struct sockaddr*) &server_addr, sizeof(server_addr));
	
	//listen for the client server
	listen(server_socket, 5);
	
	//accept a client socket
	int client_socket = accept(server_socket, NULL, NULL);
	
	//read string from the client
	char buffer[256], temp;
	bzero(buffer,256);
	int re = read(client_socket,buffer,255);
	printf("\nThe message recieved : %s",buffer);
	

	
	//send the string to client
	int wr = write(client_socket,buffer, sizeof(buffer));
	
	close(server_socket);
	
	return 0;
}
