#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "user_interaction.h"

#include "../Shared/read_packets.h"
#include "../Shared/constants.h"
#include "../Shared/packets.h"

int main() {
    struct sockaddr_in server_control_addr;
    int server_control_length = sizeof(server_control_addr);

    struct sockaddr_in server_data_addr;
    int server_data_length = sizeof(server_control_addr);

    int socket_fd;
    int8_t client_buf[DATA_PACKET_MAX_LENGTH];
    memset(client_buf, '\0', sizeof(client_buf));

    // make socket
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("couldn't build socket");
        return 1;
    }

    server_control_addr.sin_family = AF_INET;
    server_control_addr.sin_port = htons(TFTP_PORT);
    server_control_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        char request_type = inquire_request_type();
        char * file_name = inquire_file_name();

        struct request_packet * packet = build_request_packet(
                request_type,
                file_name,
                MODE_NETASCII);
        struct packet_meta * packet_meta =  build_request_frame(packet);

        ssize_t sent_bytes;
        if ((sent_bytes = sendto(
                    socket_fd,
                    packet_meta->ptr,
                    packet_meta->length,
                    0,
                    (struct sockaddr *) &server_control_addr,
                    (socklen_t) server_control_length))
                == -1) {
            printf("Problem sending stuff: %i \n", errno);
		}
        printf("received guack: \n");
        for (int i = 0; i<packet_meta->length; i++) {
            printf("byte %i: %i\n", i, *(packet_meta->ptr + i));
        }
        free_request_packet(packet);
        free_packet_meta(packet_meta);

        ssize_t recv_len;
        if ((recv_len = recvfrom(
                    socket_fd,
                    client_buf,
                    REQUEST_PACKET_LENGTH,
                    0,
                    (struct sockaddr *) &server_data_addr,
                    (socklen_t *) &server_data_length))
                == -1) {
            printf("Failed receiving data \n");
            break;
        }
        printf("received %zi bytes \n", recv_len);
    }
}
