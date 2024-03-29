/**
 * \file            user_interaction.h
 * \brief           User interaction header file
 */
#ifndef USER_INTERACTION_H_
#define USER_INTERACTION_H_

#include <stdlib.h>
#include <stdint.h>

uint16_t inquire_request_type();
char * inquire_file_name(uint16_t request_type);
char * inquire_mode();

int set_stdin_nonblocking();
int set_stdin_blocking();
int clear_stdin_for_getchar();

#endif
