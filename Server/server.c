#include <sys/socket.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#include <errno.h>
#include <pthread.h>

#include "connection_establishment.h"
#include "data_channel.h"
#include "../Shared/read_packets.h"
#include "../Shared/packets.h"

int main() {
    int control_socket;
    ssize_t recv_len;

    struct sockaddr_in client_addr;
    int client_length = sizeof(client_addr);

    if ((control_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("couldn't create socket \n");
        return -1;
    }

    if (bind_control_socket(control_socket) == -1) {
        return -1;
    }

    while (1) {
        uint8_t * buf = malloc(DATA_PACKET_MAX_LENGTH);

        pthread_t data_thread;
        int thread_no;
        struct request_params request_params;

        memset(buf, 0, DATA_PACKET_MAX_LENGTH);
        if ((recv_len = recvfrom(
                        control_socket,
                        buf,
                        REQUEST_PACKET_LENGTH,
                        0,
                        (struct sockaddr *) &client_addr,
                        (socklen_t *) &client_length))
                    == -1) {
            printf("Failed receiving data \n");
            continue;
        }
        printf("received %zu bytes \n", recv_len);

        request_params.buf = buf;
        request_params.buf_length = recv_len;
        request_params.client_addr = &client_addr;
        request_params.client_length = client_length;

        thread_no = pthread_create(&data_thread, NULL, handle_request, (void *) &request_params);
    }

    close(control_socket);
}
