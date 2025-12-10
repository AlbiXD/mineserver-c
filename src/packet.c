#include "../include/packet.h"

int packet_handler(Client *c)
{
    unsigned char* buf = c->packet_buf;
    ssize_t n = read(c->cfd, buf, BUFSIZE);
    uint8_t packID = buf[0];

    printf("%d\n", packID);
    switch (packID)
    {
    case 0x02:
        printf("Handshake Protocol!\n");
        handle_handshake(c, buf);
        break;
    case 0x01:
        printf("Login Protocol!\n");
        handle_login(c, buf);
        break;
    case 0x0D:
        handle_pos(c, buf);
        break;
    default:
        printf("Unknown Packet\n");
        break;
    }
    return 0;
}

int handle_handshake(Client *c, unsigned char *buf)
{
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
}