#ifndef PACKET_H
#define PACKET_H

#include "client.h"
#include <stdint.h>
#include "protocol.h"

typedef struct {
    uint8_t packet_id;
    uint8_t* payload;
    size_t packet_length;
} client_packet;

int packet_assembler(client * cl);
int handshake_packet(client * cl);
int packet_parser(uint8_t *client_buffer, uint8_t *packet_id, size_t bufflen, size_t bytes_read);


#endif