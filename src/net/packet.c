// This file will be responsible for assembling a packet
#include "../includes/packet.h"

// int packet_parser(client* cl, client_packet packet){

// }
const packet_id_t handshake_map[] = {
    KEEP_ALIVE,
    HANDSHAKE,
    LOGIN};

int packet_assembler(client *cl)
{
    packet_t packet;

    uint8_t *client_buffer = cl->client_buffer;
    uint8_t *packet_ptr = client_buffer + cl->packet_len;
    int size = 0;
    size_t bytes_read = cl->bytes_read;
    packet_status_t rval = PACKET_OK;

    while (1)
    {

        packet_ptr += size;
        cl->packet_len = size;

        printf("offset=%td, size=%d\n", packet_ptr - client_buffer, size); // We have consumed all bytes
        if ((size_t)(packet_ptr - client_buffer) >= bytes_read){
            return NEED_DATA;
        }

        size = 0;

        printf("packet_id=%d\n", *packet_ptr);
        // Array lookup table
        if ((*packet_ptr) > ENUM_LENGTH)
        {
            printf("Unknown Packet Type");
            return PACKET_ERROR;
        }

        if ((size = packet_length(client_buffer, packet_ptr, &cl->bytes_read, *packet_ptr)) < 0)
            return size;

        packet_init(&packet, *packet_ptr, packet_ptr, size);

        rval = packet_parser(&packet);
        packet_dispatcher(cl, *packet_ptr);
    }

    return rval;
}

void packet_dispatcher(client *cl, packet_id_t id)
{
    switch (id)
    {
    case HANDSHAKE:
    {
        printf("Handling handshake\n");
        unsigned char pkt_offline[] = {
            0x02,
            0x00, 0x01,
            0x00, 0x2D};

        write(cl->client_fd, pkt_offline, 5);
        break;
    }
    case LOGIN:
        printf("Login Packet\n");
        break;
    default:
        return;
    }

    return;
}

int packet_parser(packet_t *packet)
{
    return PACKET_OK;
}

int packet_init(packet_t *packet, packet_id_t id, uint8_t *payload, size_t packet_length)
{
    packet->id = id;
    packet->payload = payload;
    packet->packet_length = packet_length;
    return PACKET_OK;
}

int packet_length(uint8_t *client_buffer, uint8_t *packet_pointer, size_t *bytes_read_ptr, packet_id_t packet_id)
{
    size_t bytes_read = *bytes_read_ptr;
    size_t offset = packet_pointer - client_buffer;
    // bytes_read
    int size = 0;
    size_t remaining_bytes = bytes_read - offset;
    switch (packet_id)
    {
    case LOGIN:
    {
        printf("Handling login\n");
        return PACKET_ERROR;
    }
    case HANDSHAKE:
    {

        // Do we have the minimum header
        //  Do I have minimum bytes for header?
        int r = 0;
        if ((r = packet_header_check(offset, remaining_bytes, 3)) == BUFFER_CONSUMED)
        {
            memmove(client_buffer, packet_pointer, remaining_bytes + 1);
            *bytes_read_ptr = remaining_bytes;
            return PACKET_INCOMPLETE;
        }
        else if (r == HEADER_INCOMPLETE) return PACKET_INCOMPLETE; // need to change the 3 into enumerated type

        // We have minimum header
        size = packet_pointer[2] * 2 + 3;

        // Do I have minimum bytes for header?
        if (offset + size > BUFFER_LENGTH)
        {
            memmove(client_buffer, packet_pointer, remaining_bytes + 1);
            *bytes_read_ptr = remaining_bytes;
            return PACKET_INCOMPLETE;
        }

        // // YESc
        // // Is my data valid beyond this pointer?
        if (remaining_bytes < ((size_t)size) - 1)
            return PACKET_INCOMPLETE;

        return size;
    }

    default:
        printf("Unknown Packet Type\n");
        return PACKET_ERROR;
    }
}

void packet_mmove(uint8_t *client_buffer, uint8_t *packet, size_t packet_len)
{
    memmove(client_buffer, packet, packet_len);
}

int packet_header_check(size_t offset, size_t remaining_bytes, size_t header_size)
{
    // Do we have the minimum header
    //  Do I have minimum bytes for header?
    if (offset + header_size > BUFFER_LENGTH) // Do we have enough bytes?
        return BUFFER_CONSUMED;
    // Is my data valid beyond this pointer? 0A 0B 0C 02 00 06 5-4 == 1
    // offset = 4
    // bytes_read = 5
    // 5-4 == 1 meaning there is one byte that is valid
    if (remaining_bytes < header_size) // if so we need 2 bytes ahead of this that are valid
        return HEADER_INCOMPLETE;

    return HEADER_OK;
}