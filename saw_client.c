#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define TIMEOUT 3 // Timeout duration in seconds

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <total_frames>\n", argv[0]);
        return 1;
    }

    int total_frames = atoi(argv[1]);
    int next_frame = 0;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;

    // Set socket timeout
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    while (next_frame < total_frames) {
        printf("Sending frame: %d\n", next_frame);
        sendto(sockfd, &next_frame, sizeof(next_frame), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        int ack_frame;
        socklen_t len = sizeof(servaddr);
        int n = recvfrom(sockfd, &ack_frame, sizeof(ack_frame), 0, (struct sockaddr *)&servaddr, &len);

        if (n < 0) {
            printf("Timeout occurred for frame %d. Resending...\n", next_frame);
            continue;
        }

        if (ack_frame == -1) {
            printf("Received NAK for frame %d. Resending...\n", next_frame);
            continue;
        }

        if (ack_frame == next_frame + 1) {
            next_frame++;
        }
    }

    close(sockfd);
    return 0;
}
