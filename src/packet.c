#include "../include/packet.h"


int read_packet(Client *c ,unsigned char *buf, int n)
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
            break;

        if (r < 0)
        {
            // nothing left for now
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            // real error
            break;
        }
    }

    if (r == 0) return -1; // client actually closed

        return total_read;
    }

int packet_handler(Client *c)
{
    //Read once initially
    unsigned char *buf = c->packet_buf;
    int n = 0; 
    printf("Here\n");
    printf("Extra = %d, n = %d\n", c->packet_len, n);

    n = read_packet(c, buf+c->packet_len, n-c->packet_len); 

    if(n == 0) return 0;
    if(n == -1) return -1;


    //Get packetID
    uint8_t packID = buf[0];

    //
    int retval = 0;

    printf("%d\n", packID);
    switch (packID)
    {
    case 0x02:
        printf("Handshake Protocol!\n");
        handle_handshake(c, buf, n);
        retval = 0x02;
        break;
    case 0x01:
        printf("Login Protocol!\n");
        handle_login(c, buf);
        retval = 0x01;
        break;
    case 0x0D:
        handle_pos(c, buf);
        retval = 0x0D;
        break;
    default:
        printf("Unknown Packet\n");
        retval = 0xFFF;
        break;
    }

    printf("Here\n");
    return retval;
}

int handle_handshake(Client *c, unsigned char *buf, int n)
{
    if(n < 3) n = read_packet(c, buf, n); //should have read an extra byte at least?
   
    //size of string
    uint8_t len = buf[2];

    //packet_size
    int size = 3 + len*2;
    
    //Read until we get the full packet
    while(n < size) n = read_packet(c, buf, n);

    //We got full packet + potentially next extra byte
    int extra = n-size;

    printf("Extra in handshake %d\n", extra);

    if(extra != 0){
        memmove(buf, buf+size, extra);
        c->packet_len = extra;
    }  


    c->state = STATE_HANDSHAKE_START; // Handshake initiated



    unsigned char pkt_offline[] = {
        0x02,
        0x00, 0x01,
        0x00, 0x2D
    };

    write(c->cfd, pkt_offline, 5);
    return 0;
}

int handle_login(Client *c, unsigned char *buf)
{
    c->state = STATE_LOGIN_START;
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