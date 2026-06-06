#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

/*
Will return packet size that was parsed in bytes 
*/
int parse_handshake(uint8_t *client_buffer, uint8_t *packet_id, size_t bufflen, size_t bytes_read);


#endif