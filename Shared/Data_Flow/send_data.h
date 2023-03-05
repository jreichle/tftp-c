#ifndef SEND_DATA_H_
#define SEND_DATA_H_

#include <arpa/inet.h>
#include <sys/socket.h>

#include "../Packet_Manipulation/packets.h"

void send_file(
        struct request_packet * request,
        int * socket,
        struct sockaddr_in * address,
        int address_length);

int send_packet(
        uint8_t * data,
        struct data_packet * data_packet,
        uint16_t block_number,
        int * socket,
        uint8_t * ack_buf,
        struct sockaddr_in * address,
        int address_length);

int send_buffer(
        int * socket,
        uint8_t * buf,
        int buf_length,
        struct sockaddr_in * address,
        int address_length);

#endif
