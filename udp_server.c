#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h> // Corrected header file for socket programming

#define port 9001
#define BUFFER_SIZE 256

int main()
{
    int server_socket;
    struct sockaddr_in server_addr, client_addr; // Structure to hold server and client address information
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];
    
    // Creating a socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0)
    {
        perror("ERROR opening socket\n");
    }
    
    // Assigning address and port to server socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // Binding the socket to the port number
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("ERROR binding socket\n");
    }
    
    addr_size = sizeof(client_addr);
    memset(buffer, 0, BUFFER_SIZE); 
    
    // Receiving data 
    int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_size);
    if (bytes_received < 0)
    {
        perror("ERROR receiving data\n");
    }
    printf("Received message: %s\n", buffer);
    
    // Sending the received data back to the client 
    int bytes_sent = sendto(server_socket, buffer, bytes_received, 0, (struct sockaddr*)&client_addr, addr_size);
    if (bytes_sent < 0)
    {
        perror("ERROR sending data\n");
    }
    
    // Closing the socket
    close(server_socket);
    return 0;
}

