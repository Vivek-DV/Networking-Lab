#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 9001

struct data {
    int key;
    char value[256];
} pair[] = {
    {1, "Vivek"},
    {2, "Ajith"},
    {3, "Veena"},
    {4, "Vidya"},
    {5, "Aiswarya"}
};

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int k, found = 0;
    char buffer[256];

    // Socket creating
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) 
    {
        perror("ERROR opening socket");
    }

    // Assign address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port number
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("ERROR on binding");
        exit(1);
    }

    // Listen for client connections
    listen(server_socket, 5);

    // Accept client connection
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    // Read key from client
    bzero(buffer, 256);
    if (read(client_socket, &k, sizeof(k)) < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("Received Key: %d\n", k);

    // Search for the key
    for (int i = 0; i < 5; i++) {
        if (pair[i].key == k) {
            if (write(client_socket, pair[i].value, strlen(pair[i].value) + 1) < 0) {
                perror("ERROR writing to socket");
            }
            printf("Sent Value: %s\n", pair[i].value);
            found = 1;
            break;
        }
    }
    if (!found) {
        char *not_found_msg = "Key not found";
        write(client_socket, not_found_msg, strlen(not_found_msg) + 1);
        printf("Sent: Key not found\n");
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}

