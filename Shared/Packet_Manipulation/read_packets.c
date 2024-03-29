/**
 * \file            read_packets.c
 * \brief           Handle incoming packets and convert them to the resulting struct
 */
#include "read_packets.h"

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>

#include "packets.h"
#include "../utils.h"


/**
 * \brief           extract the opcode of a frame
 * \param[in]       frame: incoming data frame
 * \return          the opcode of a frame
 */
int identify_packet_type(uint8_t * frame) {
    uint16_t opcode = ntohs(uint8_buf_to_uint16(frame));
    if (opcode >= 0 && opcode <= 5) {
        return opcode;
    } else {
        return -1;
    }
}


/**
 * \brief           convert a buffer to a request_packet struct
 * \param[in]       buf: Pointer to a buffer, which is the base for the request packet
 * \param[in]       received_bytes: size of the buffer
 * \return          request_packet or NULL for invalid request_packets
 */
struct request_packet * convert_buf_to_request_packet(uint8_t * buf, ssize_t received_bytes) {
    int packet_type;
    struct request_packet * request;
    int offset;

    char * strings;
    char * first_string;
    char * second_string;

    if (received_bytes < REQUEST_PACKET_MIN_LENGTH) {
        return NULL;
    }
    packet_type = identify_packet_type(buf);
    if (packet_type != OPCODE_RRQ && packet_type != OPCODE_WRQ) {
        return NULL;
    }

    offset = 0;
    request = (struct request_packet *) malloc(sizeof(struct request_packet));
    request->opcode = packet_type;
    offset += OPCODE_LENGTH;
    strings = (char *) buf;
    first_string = strtok(strings + offset, "0");
    request->file_name = malloc(strlen(first_string));
    memcpy(request->file_name, first_string, strlen(first_string));
    offset += strlen(first_string) + ZERO_BYTE_LENGTH;
    second_string = strtok(strings + offset, "0");
    request->mode = malloc(strlen(second_string));
    memcpy(request->mode, second_string, strlen(second_string));
    return request;
}

/**
 * \brief           convert a buffer to a data_packet struct
 * \param[in]       buf: Pointer to a buffer, which is the base for the data packet
 * \param[in]       received_bytes: size of the buffer
 * \return          request_packet or NULL for invalid data_packets
 */
struct data_packet * convert_buf_to_data_packet(uint8_t * buf, ssize_t received_bytes) {
    int packet_type;
    struct data_packet * data_packet;
    int offset;
    int data_length;

    if (received_bytes < DATA_PACKET_MIN_LENGTH) {
        return NULL;
    }
    packet_type = identify_packet_type(buf);
    if (packet_type != OPCODE_DATA) {
        return NULL;
    }

    offset = 0;
    data_packet = (struct data_packet *) malloc(sizeof(struct data_packet));
    data_packet->opcode = packet_type;
    offset += OPCODE_LENGTH;
    data_packet->block_no = uint8_buf_to_uint16(buf + offset);
    offset += BLOCK_NO_LENGTH;
    data_length = received_bytes - offset;
    data_packet->data_length = data_length;
    data_packet->data = malloc(sizeof(uint8_t) * data_length);
    memcpy(data_packet->data, buf + offset, data_length);

    return data_packet;
}

/**
 * \brief           convert a buffer to a ack_packet struct
 * \param[in]       buf: Pointer to a buffer, which is the base for the acknowledge packet
 * \param[in]       received_bytes: size of the buffer
 * \return          request_packet or NULL for invalid ack_packets
 */
struct ack_packet * convert_buf_to_ack_packet(uint8_t * buf, ssize_t received_bytes) {
    int packet_type;
    struct ack_packet * ack_packet;
    int offset;

    if (received_bytes < ACK_PACKET_LENGTH) {
        return NULL;
    }
    packet_type = identify_packet_type(buf);
    if (packet_type != OPCODE_ACK) {
        return NULL;
    }

    offset = 0;
    ack_packet = (struct ack_packet *) malloc(sizeof(struct ack_packet));
    ack_packet->opcode = packet_type;
    offset += OPCODE_LENGTH;
    ack_packet->block_no = uint8_buf_to_uint16(buf + offset);

    return ack_packet;
}

/**
 * \brief           convert a buffer to a error_packet struct
 * \param[in]       buf: Pointer to a buffer, which is the base for the error packet
 * \param[in]       received_bytes: size of the buffer
 * \return          request_packet or NULL for invalid error_packets
 */
struct error_packet * convert_buf_to_error_packet(uint8_t * buf, ssize_t received_bytes) {
    int packet_type;
    struct error_packet * error_packet;
    int offset;
    char * error_message;

    if (received_bytes < ERROR_PACKET_MIN_LENGTH) {
        return NULL;
    }
    packet_type = identify_packet_type(buf);
    if (packet_type != OPCODE_ERROR) {
        return NULL;
    }

    offset = 0;
    error_packet = (struct error_packet *) malloc(sizeof(struct error_packet));
    error_packet->opcode = packet_type;
    offset += OPCODE_LENGTH;
    error_packet->error_code = uint8_buf_to_uint16(buf + offset);
    offset += ERROR_CODE_LENGTH;
    error_message = strtok((char *) buf + offset, "0");
    error_packet->error_message = malloc(strlen(error_message));
    memcpy(error_packet->error_message, error_message, strlen(error_message));

    return error_packet;
}
