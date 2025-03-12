#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define WINDOW_SIZE 4

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <total_frames>\n", argv[0]);
        exit(1);
    }

    int total_frames = atoi(argv[1]);
    int next_frame = 0, ack_frame;
    int sockfd;
    struct sockaddr_in server_addr;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Server address setup
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    while (next_frame < total_frames) {
        int remaining_frames = ((next_frame + WINDOW_SIZE) < total_frames) ? WINDOW_SIZE : total_frames - next_frame;

        for (int i = 0; i < remaining_frames; i++) {
            printf("Sent frame %d\n", next_frame);
            sendto(sockfd, &next_frame, sizeof(next_frame), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
            next_frame++;
        }

        // Receive ACK from the server
        recvfrom(sockfd, &ack_frame, sizeof(ack_frame), 0, NULL, NULL);
        printf("Received ACK %d\n", ack_frame);

        // Update next_frame based on the ACK
        next_frame = ack_frame;
    }

    close(sockfd);
    printf("Client finished.\n");
    return 0;
}



