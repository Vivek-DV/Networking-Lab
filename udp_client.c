#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

#define server_IP "127.0.0.1"
#define port 9001
#define BUFFER_SIZE 256

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];
    
    // Creating client socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0)
    {
        perror("ERROR creating socket\n");
        return -1;
    }
    
    // Assigning address and port to the socket
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    addr_size = sizeof(server_addr);
    
    printf("Enter message: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    
    // Sending message to server
    sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_size);
    memset(buffer, 0, BUFFER_SIZE);
    
    // Receiving response from server
    recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_size);
    printf("Message from server: %s", buffer);
    
    // Closing the socket
    close(client_socket);
    return 0;
}

