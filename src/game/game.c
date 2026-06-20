#include "../includes/game.h"
#include "../includes/packet.h"
#include "../includes/client.h"
int GAME_CommandHandler(command_t *cmd)
{

    printf("GAME HANDLER\n");

    int command_id = cmd->id;

    if (cmd->id == 0)
        return 0;
    int fd = cmd->sender->net.fd;

    client *sender = cmd->sender;
    switch (command_id)
    {
    case HANDSHAKE:
    {
        if (sender->state != CLIENT_HANDSHAKE)
            return STATE_ERROR;
        uint8_t payload[] = {0x02, 0x00, 0x01, 0x00, 0x2D};

        write(fd, payload, sizeof(payload));
        printf("Wrote handshake packet\n");
        sender->state = CLIENT_LOGIN;
        return 0;
    }
    case LOGIN:
    {

        if (sender->state != CLIENT_LOGIN)
            return STATE_ERROR;
        printf("LOGIN\n");
        uint8_t payload[] = {
            0x01,

            0x00, 0x00, 0x05, 0x12,

            0x00, 0x00,

            0x0D, 0x7C, 0x58, 0xD8, 0x8C, 0x4A, 0x91, 0xA,

            0x00};

        write(fd, payload, 16);
        printf("Wrote login packet\n");

        unsigned char pkt_poslook[] = {
            0x0D,

            // X
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,

            // Y
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,

            // Stance
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,

            // Z
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,

            // Yaw
            0x00, 0x00, 0x00, 0x00,

            // Pitch
            0x00, 0x00, 0x00, 0x00,

            // OnGround
            0x00};

        write(fd, pkt_poslook, sizeof(pkt_poslook));
        return 0;
    }
    default:
        return 0;
    }

    return 0;
}
