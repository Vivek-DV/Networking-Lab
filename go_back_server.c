#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define WINDOW_SIZE 4
#define MAX_FRAMES 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <total_frames>\n", argv[0]);
        exit(1);
    }

    int total_frames = atoi(argv[1]);
    int lost_frames[MAX_FRAMES], num_lost = 0;
    int next_frame = 0, corrupted_frame = -1;
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Bind the socket
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("Server started. Waiting for frames...\n");

    // Read the number of lost frames
    printf("Enter the number of lost frames: ");
    scanf("%d", &num_lost);

    if (num_lost > 0) {
        printf("Enter the frame numbers of lost frames: ");
        for (int i = 0; i < num_lost; i++) {
            scanf("%d", &lost_frames[i]);
        }
    }

    while (next_frame < total_frames) {
        int remaining_frames = ((next_frame + WINDOW_SIZE) < total_frames) ? WINDOW_SIZE : total_frames - next_frame;

        for (int i = 0; i < remaining_frames; i++) {
            int recv_frame;
            recvfrom(sockfd, &recv_frame, sizeof(recv_frame), 0, (struct sockaddr *)&client_addr, &addr_len);
            printf("Received frame %d\n", recv_frame);

            // Check if the frame is corrupted
            if (corrupted_frame == -1) {
                for (int j = 0; j < num_lost; j++) {
                    if (recv_frame == lost_frames[j]) {
                        corrupted_frame = recv_frame;
                        printf("Corrupted frame %d\n", corrupted_frame);
                        break;
                    }
                }
            }
        }

        if (corrupted_frame != -1) {
            next_frame = corrupted_frame;
            // Remove the corrupted frame from the lost list
            for (int i = 0; i < num_lost; i++) {
                if (lost_frames[i] == corrupted_frame) {
                    for (int j = i; j < num_lost - 1; j++) {
                        lost_frames[j] = lost_frames[j + 1];
                    }
                    num_lost--;
                    break;
                }
            }
            corrupted_frame = -1;
        } else {
            next_frame += remaining_frames;
        }

        // Send ACK to the client
        sendto(sockfd, &next_frame, sizeof(next_frame), 0, (struct sockaddr *)&client_addr, addr_len);
        printf("Sent ACK %d\n", next_frame);
    }

    close(sockfd);
    printf("Server finished.\n");
    return 0;
}
