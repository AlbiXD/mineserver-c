#ifndef PACKET_H
#define PACKET_H

#include "client.h"
#include <stdint.h>
#include <arpa/inet.h>
#include "cmd_queue.h"
#define ENUM_LENGTH 3

typedef enum
{
    KEEP_ALIVE = 0x00,
    LOGIN = 0x01,
    HANDSHAKE = 0x02,
    POSITION = 0x0B,
    LOOK = 0x0C,
    POSITION_AND_LOOK = 0x0D,
    DISCONNECT = 0xFF,
    ON_GROUND = 0x0A
} packet_id_t;

typedef struct
{
    packet_id_t id;
    size_t minSize;
    int sizeOffset;
    uint8_t isSized; // If this variable consists of a dynamic size
} packet_meta_t;

extern const packet_meta_t PLTB[256];

typedef struct
{
    packet_id_t id;
    uint8_t *payload;
    size_t packet_length;
} packet_t;

typedef enum
{
    TESTING = 100,
    HEADER_OK = 1,
    PACKET_OK = 0,
    PACKET_INCOMPLETE = -1,
    PACKET_COMPACTED = -2,
    PACKET_ERROR = -3,
    BUFFER_CONSUMED = -4,
    NEED_DATA = -5,
    PACKET_UNSUPPORTED = -6,
    PACKET_DISCONNECT = -7
} packet_status_t;

int PKT_Assemble(server *srv, client *cl, cmd_queue *queue);
void PKT_Mmove(uint8_t *client_buffer, uint8_t *packet, size_t packet_len);
int PKT_LengthCheck(size_t offset, size_t remaining_bytes, size_t size);
int PKT_Parser(server *srv, packet_t *packet, client *sender, cmd_queue *queue);
int PKT_Init(packet_t *packet, packet_id_t id, uint8_t *payload, size_t packet_length);
int PKT_Length(uint8_t *client_buffer, uint8_t *packet_pointer, size_t *bytes_read_ptr, packet_id_t packet_id);
#endif