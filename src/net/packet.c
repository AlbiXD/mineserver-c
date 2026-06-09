// This file will be responsible for assembling a packet
#include "../includes/packet.h"

// int handshake_packet(client *cl)
// {

//     unsigned char pkt_offline[] = {
//         0x02,
//         0x00, 0x01,
//         0x00, 0x2D};

//     write(cl->client_fd, pkt_offline, 5);
//     return 0;
// }

// int packet_parser(client* cl, client_packet packet){

// }

int packet_assembler(client *cl)
{
    packet_t packet;

    uint8_t *client_buffer = cl->client_buffer;
    uint8_t *packet_ptr = client_buffer + cl->packet_len;
    int size = 0;
    size_t bytes_read = cl->bytes_read;
    int packet_id = 0;
    size_t bytes_behind = 0;
    packet_status_t rval = PACKET_OK;

    while (1)
    {

        bytes_behind = packet_ptr - client_buffer;

        packet_ptr += size;
        cl->packet_len = size;

        printf("offset=%td, size=%d\n", packet_ptr - client_buffer, size);
        if(packet_ptr-client_buffer >= bytes_read) return PACKET_INCOMPLETE;


        size = 0;

        switch (*packet_ptr)
        {
        case HANDSHAKE:
        {
            printf("Handshake Protocol\n");
            packet_id = HANDSHAKE;
            break;
        }

        case LOGIN:
        {
            printf("Login Protocol\n");
            break;
        }

        default:
            printf("Unknown Packet Type\n");
            return PACKET_ERROR;
        }

        if ((size = packet_length(client_buffer, packet_ptr, &cl->bytes_read, packet_id,  bytes_behind)) < 0){
            rval = size;
            break;
        }

        packet_init(&packet, packet_id, packet_ptr, size);

        
        // packet_parser();
        // packet_dispatcher();
    }

    return rval;
}

int packet_init(packet_t *packet, packet_id_t id, uint8_t *payload, size_t packet_length)
{
    packet->id = id;
    packet->payload = payload;
    packet->packet_length = packet_length;
    return PACKET_OK;
}

int packet_length(uint8_t *client_buffer, uint8_t *packet_pointer, size_t *bytes_read_ptr, packet_id_t packet_id, size_t bytes_behind)
{
    size_t bytes_read = *bytes_read_ptr;
    size_t buffer_length = MULTIPLE * KB;
    switch (packet_id)
    {
    case HANDSHAKE:
    {
        int size = 0;

        if (bytes_behind + 3 > buffer_length)
        {
            size_t offset = packet_pointer - client_buffer;
            size_t partial_length = *bytes_read_ptr - offset;
            *bytes_read_ptr = partial_length;
            packet_mmove(client_buffer, packet_pointer, partial_length);
            return PACKET_INCOMPLETE;
        }

        if(bytes_behind + 3 > bytes_read) return PACKET_INCOMPLETE;

        if (bytes_read < 3)
            return PACKET_INCOMPLETE;

        size = 3 + packet_pointer[2] * 2;
        if (bytes_read < size)
            return PACKET_INCOMPLETE;

        printf("handling hanshake %d\n", size);

        return size;
    }
    default:
        return PACKET_ERROR;
    }
}

void packet_mmove(uint8_t *client_buffer, uint8_t *packet, size_t packet_len)
{
    memmove(client_buffer, packet, packet_len);
}