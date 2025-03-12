#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9001

int main() {
    int sock;
    struct sockaddr_in server_addr;
    int key;
    char buffer[256];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Server address configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    // Input key from user
    printf("Enter key (1-5): ");
    scanf("%d", &key);

    // Send key to server
    if (write(sock, &key, sizeof(key)) < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Read response from server
    bzero(buffer, 256);
    if (read(sock, buffer, 256) < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("Received: %s\n", buffer);

    // Close socket
    close(sock);

    return 0;
}

