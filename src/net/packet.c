// This file will be responsible for assembling a packet
#include "../includes/packet.h"

int handshake_packet(client *cl)
{

    unsigned char pkt_offline[] = {
        0x02,
        0x00, 0x01,
        0x00, 0x2D};

    write(cl->client_fd, pkt_offline, 5);
}

int packet_assembler(client *cl)
{

    uint8_t *client_buffer = cl->client_buffer;
    uint8_t *packet_id = client_buffer + cl->packet_len;
    int size = 0;

    while (1)
    {
        printf("-----------------------\n");
        packet_id += size;
        cl->packet_len = size;

        printf("read=%d\n", cl->bytes_read);
        printf("size=%d\n", size);

        if (size >= cl->bytes_read)
        {
            printf("True\n");
            printf("-----------------------\n");

            return 0;
        }
        printf("diff=%ld\n", client_buffer - packet_id);
        printf("packed_id=%d\n", *packet_id);
        size = 0;
        sleep(1);

        switch (*packet_id)
        {
        case 0x02:
        {
            printf("Handshake Protocol\n");
            size = parse_handshake(client_buffer, packet_id, KB * MULTIPLE, cl->bytes_read);
            handshake_packet(cl);
            break;
        }

        case 0x01:
        {
            printf("Login Protocol\n");
            break;
        }

        default:
            printf("Unknown Packet Type\n");
            return 0;
        }

        printf("-----------------------\n");
    }
    return 0;
}