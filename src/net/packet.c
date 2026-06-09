// This file will be responsible for assembling a packet
#include "../includes/packet.h"

int handshake_packet(client *cl)
{

    unsigned char pkt_offline[] = {
        0x02,
        0x00, 0x01,
        0x00, 0x2D};

    write(cl->client_fd, pkt_offline, 5);
    return 0;
}

// int packet_parser(client* cl, client_packet packet){

// }

int packet_assembler(client *cl)
{

    uint8_t *client_buffer = cl->client_buffer;
    uint8_t *packet_id = client_buffer + cl->packet_len;
    size_t size = 0;
    size_t bytes_read = cl->bytes_read;
    while (1)
    {
        printf("-----------------------\n");
        packet_id += size;
        cl->packet_len = size;

        printf("read=%d\n", cl->bytes_read);
        printf("size=%d\n", size);

        if (size >= cl->bytes_read) //if the size of packet is more than current bytes
            return 0;

        printf("diff=%ld\n", client_buffer - packet_id);
        printf("packed_id=%d\n", *packet_id);
        size = 0;

        switch (*packet_id)
        {
        case 0x02:
        {
            printf("Handshake Protocol\n");
            size = handshake_length(packet_id, cl->bytes_read);
            break;
        }

        case 0x01:
        {
            printf("Login Protocol\n");
            break;
        }

        default:
            printf("Unknown Packet Type\n");
            return -1;
        }

    }
    return 0;
}