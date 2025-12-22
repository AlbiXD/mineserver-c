#include "../include/packet.h"

int read_packet(Client *c, unsigned char *buf, int n)
{
    ssize_t total_read = n;
    ssize_t r = 0;

    // Drains the kernel payload buffer
    while (1)
    {
        r = read(c->cfd, buf + total_read, BUFFSIZE - total_read);

        if (r > 0)
        {
            total_read += r;
            continue;
        }

        // Person closed connection?
        if (r == 0)
        {
            printf("Peer closed connection\n");
            break;
        }

        if (r < 0)
        {
            // nothing left for now
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            // real error
            break;
        }
    }

    if (r == 0)
        return -1; // client actually closed

    return total_read;
}

/*
    Return >0 → packet length (success, full packet found)
    Return 0 → not enough data yet (keep reading)
    Return <0 → error / invalid / disconnect
*/
int packet_assembler(Client *c, unsigned char *instream, unsigned char *packet_stream, int n)
{
    // Index
    if (n <= 0)
        return 0;

    
    unsigned char pid = instream[0];
    int retval = 0;

    printf("PID: %02X\n", pid);
    switch (pid)
    {
    case 0x01:
    {
        printf("Login Protocol\n");
        if (n < 7)
            return 0;
        uint8_t length = instream[6];
        int size = 16 + length * 2;
        if (n < size)
            return 0;
        int extra = n - size;
        memmove(packet_stream, instream, size);
        memmove(instream, instream + size, extra);
        c->instream_len = extra;
        retval = size;
        break;
    }
    case 0x02:
    {
        printf("Handshake Protocol\n");
        if (n < 3)
            return 0;

        uint8_t length = instream[2];
        int size = 3 + length * 2;

        if (n < size)
            return 0;

        int extra = n - size;
        memmove(packet_stream, instream, size);
        memmove(instream, instream + size, extra);
        c->instream_len = extra;
        retval = size;
        break;
    }
    case 0x0D:
        printf("Position Protocol\n");
        if (n < 42)
            return 0;

        int extra = n - 42;
        memmove(packet_stream, instream, 42);
        memmove(instream, instream + 42, extra);
        c->instream_len = extra;
        retval = 42;
        break;

    default:
        retval = 0;
        break;
    }

    return retval;
}

int packet_handler(Client *c)
{
    // Read once initially
    unsigned char *instream = c->instream;
    unsigned char *packet = c->packet;

    int n = 0, r = 0;

    // Reads into instream
    n = read_packet(c, instream, c->instream_len);

    if(n < 0) return -1;
    // Sets the length of the instream to total bytes read
    c->instream_len = n;

    // Assemble the packet until packet is partial then read more
    while ((r = packet_assembler(c, instream, packet, n)) > 0)
    {
        packet_dump(packet, r);
        unsigned char pid = packet[0];
        switch (pid)
        {
        case 0x01:
            handle_login(c, packet);
            break;
        case 0x02:
            handle_handshake(c, packet);
            break;
        case 0x0D:
            handle_pos(c, packet);
            break;
        default:
            break;
        }
        n = c->instream_len;
    }
    printf("R value = %d\n", r);

    return r;
}
int handle_chat(Client *c, unsigned char *buf)
{
    // Displays the message

    return 0;
}
int handle_handshake(Client *c, unsigned char *buf)
{
    unsigned char pkt_offline[] = {
        0x02,
        0x00, 0x01,
        0x00, 0x2D};

    write(c->cfd, pkt_offline, 5);
    return 0;
}
int handle_login(Client *c, unsigned char *buf)
{
    unsigned char pkt_login[] = {
        0x01,

        0x00, 0x00, 0x05, 0x12,

        0x00, 0x00,

        0x0D, 0x7C, 0x58, 0xD8, 0x8C, 0x4A, 0x91, 0xA,

        0x00};
    write(c->cfd, pkt_login, 16);

    return 0;
}
int handle_pos(Client *c, unsigned char *buf)
{
    unsigned char pkt_poslook[] = {
        0x0D,

        0x40, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        0x40, 0x50, 0xCF, 0x5C, 0x29, 0x00, 0x00, 0x00,

        0x40, 0x50, 0x67, 0xAE, 0x14, 0x80, 0x00, 0x00,

        0x40, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00,

        0x00};

    write(c->cfd, pkt_poslook, sizeof(pkt_poslook));

    return 0;
}