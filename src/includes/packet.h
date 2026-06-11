#ifndef PACKET_H
#define PACKET_H

#include "client.h"
#include <stdint.h>

#define ENUM_LENGTH 3

typedef enum {
    KEEP_ALIVE = 0,
    LOGIN = 1,
    HANDSHAKE = 2
} packet_id_t;

extern const packet_id_t handshake_map[];

typedef struct {
    packet_id_t id;
    uint8_t* payload;
    size_t packet_length;
} packet_t;


typedef enum {
    HEADER_OK = 1,
    PACKET_OK = 0,
    PACKET_INCOMPLETE = -1,
    PACKET_COMPACTED = -2,
    PACKET_ERROR = -3,
    BUFFER_CONSUMED = -4,
    HEADER_INCOMPLETE = -5,
    NEED_DATA = -6
} packet_status_t;

int packet_assembler(client * cl);
void packet_dispatcher(client * cl, packet_id_t id);
void packet_mmove(uint8_t *client_buffer, uint8_t *packet, size_t packet_len);
int packet_header_check(size_t offset, size_t remaining_bytes, size_t header_size);
int packet_parser(packet_t *packet);
int packet_init(packet_t *packet, packet_id_t id, uint8_t *payload, size_t packet_length);
int packet_length(uint8_t *client_buffer, uint8_t *packet_pointer, size_t* bytes_read_ptr, packet_id_t packet_id);
#endif