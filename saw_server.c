#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define MAXLEN 1024

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <total_frames> <num_lost> <num_timeout>\n", argv[0]);
        return 1;
    }

    int total_frames = atoi(argv[1]);
    int num_lost = atoi(argv[2]);
    int num_timeout = atoi(argv[3]);

    int lost_list[num_lost], timeout_list[num_timeout];

    printf("Enter lost frame numbers: ");
    for (int i = 0; i < num_lost; i++)
        scanf("%d", &lost_list[i]);

    printf("Enter timeout frame numbers: ");
    for (int i = 0; i < num_timeout; i++)
        scanf("%d", &timeout_list[i]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    int next_frame = 0;
    socklen_t len = sizeof(cliaddr);

    while (next_frame < total_frames) {
        int recv_frame;
        recvfrom(sockfd, &recv_frame, sizeof(recv_frame), 0, (struct sockaddr *)&cliaddr, &len);
        printf("Received frame: %d\n", recv_frame);

        int is_lost = 0;
        for (int i = 0; i < num_lost; i++) {
            if (recv_frame == lost_list[i]) {
                printf("Frame %d lost, sending NAK.\n", recv_frame);
                int nak = -1;
                sendto(sockfd, &nak, sizeof(nak), 0, (struct sockaddr *)&cliaddr, len);
                lost_list[i] = -1;
                is_lost = 1;
                break;
            }
        }

        if (is_lost) continue;

        for (int i = 0; i < num_timeout; i++) {
            if (recv_frame == timeout_list[i]) {
                printf("Frame %d timeout, ignoring.\n", recv_frame);
                timeout_list[i] = -1;
                is_lost = 1;
                break;
            }
        }

        if (is_lost) continue;

        next_frame = recv_frame + 1;
        printf("Sending ACK: %d\n", next_frame);
        sendto(sockfd, &next_frame, sizeof(next_frame), 0, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}

