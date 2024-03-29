/**
 * \file            write_packets.c
 * \brief           Prepare packets for sending
 */
#include "write_packets.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "../utils.h"

/**
 * \brief           build a request frame from the information given as a parameter
 * \param[in]       information: necessary information to build request packet
 * \return          Packet Metadaten: the Pointer to the frame and the length of it
 */
packet_meta * build_request_frame(request_packet * information) {
    int file_name_length = strlen(information->file_name);
    int mode_length = strlen(information->mode);

    struct packet_meta * meta = malloc(sizeof(struct packet_meta));
    meta->length = OPCODE_LENGTH + file_name_length
        + ZERO_BYTE_LENGTH + mode_length + ZERO_BYTE_LENGTH;
    meta->ptr = malloc(meta->length);
    memset(meta->ptr, 0, meta->length);

    int offset = 0;
    uint16_to_uint8_buf(htons(information->opcode), meta->ptr);
    offset += OPCODE_LENGTH;
    memcpy(meta->ptr + offset, information->file_name, file_name_length);
    offset += file_name_length;
    memset(meta->ptr + offset, 0, ZERO_BYTE_LENGTH);
    offset += ZERO_BYTE_LENGTH;
    memcpy(meta->ptr + offset, information->mode, mode_length);
    offset += mode_length;
    memset(meta->ptr + offset, 0, ZERO_BYTE_LENGTH);

    return meta;
}

/**
 * \brief           build a data frame from the information given as a parameter
 * \param[in]       information: necessary information to build data packet
 * \return          Packet Metadaten: the Pointer to the frame and the length of it
 */
packet_meta * build_data_frame(data_packet * information) {
    struct packet_meta * meta = malloc(sizeof(struct packet_meta));
    meta->length = OPCODE_LENGTH + BLOCK_NO_LENGTH + information->data_length;
    meta->ptr = malloc(sizeof(uint8_t) * meta->length);

    int offset = 0;
    uint16_to_uint8_buf(htons(information->opcode), meta->ptr);
    offset += OPCODE_LENGTH;
    uint16_to_uint8_buf(htons(information->block_no), meta->ptr + offset);
    offset += BLOCK_NO_LENGTH;
    memcpy(meta->ptr + offset, information->data, information->data_length);

    return meta;
}

/**
 * \brief           build an acknowledge frame from the information given as a parameter
 * \param[in]       information: necessary information to build acknowledge packet
 * \return          Packet Metadaten: the Pointer to the frame and the length of it
 */
packet_meta * build_ack_frame(ack_packet * information) {
    struct packet_meta * meta = malloc(sizeof(struct packet_meta));
    meta->length = OPCODE_LENGTH + BLOCK_NO_LENGTH;
    meta->ptr = malloc(meta->length);

    int offset = 0;
    uint16_to_uint8_buf(htons(information->opcode), meta->ptr);
    offset += OPCODE_LENGTH;
    uint16_to_uint8_buf(htons(information->block_no), meta->ptr + offset);

    return meta;
}

/**
 * \brief           build an error frame from the information given as a parameter
 * \param[in]       information: necessary information to build error packet
 * \return          Packet Metadaten: the Pointer to the frame and the length of it
 */
packet_meta * build_error_frame(error_packet * information) {
    int error_message_length = strlen(information->error_message);

    struct packet_meta * meta = malloc(sizeof(struct packet_meta));
    meta->length = OPCODE_LENGTH + BLOCK_NO_LENGTH + error_message_length + ZERO_BYTE_LENGTH;
    meta->ptr = malloc(meta->length);

    int offset = 0;
    uint16_to_uint8_buf(htons(information->opcode), meta->ptr);
    offset += OPCODE_LENGTH;
    uint16_to_uint8_buf(htons(information->error_code), meta->ptr + offset);
    offset += ERROR_CODE_LENGTH;
    memcpy(meta->ptr + offset, information->error_message, error_message_length);
    offset += error_message_length;
    memset(meta->ptr + offset, 0, ZERO_BYTE_LENGTH);
    return meta;
}
