#include "../includes/game.h"
#include "../includes/packet.h"
#include "../includes/client.h"
#include "../includes/cmd_queue.h"

int GAME_Tick(cmd_queue *queue)
{

    game_command_t *cmd;
    while ((cmd = CMDQ_Pop(queue)) != NULL)
    {
        // printf("POP goes the Weasel cmd=%d!\n", cmd->id);
        free(cmd);
    }

    return 0;
}

int GAME_Handshake(game_command_t *cmd)
{
    client *sender = cmd->sender;
    int fd = sender->net.fd;

    if (sender->state != CLIENT_HANDSHAKE)
        return STATE_ERROR;
    uint8_t payload[] = {0x02, 0x00, 0x01, 0x00, 0x2D};

    write(fd, payload, sizeof(payload));
    printf("Wrote handshake packet\n");
    sender->state = CLIENT_LOGIN;
    free(cmd);
    return 0;
}
int GAME_KeepAlive(game_command_t *cmd)
{
    uint8_t byte = 0x0;
    write(cmd->sender->net.fd, &byte, sizeof(byte));
    return 0;
}
int GAME_Login(game_command_t *cmd)
{
    client *sender = cmd->sender;
    int fd = sender->net.fd;

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

    // send pre chunk

    // send chunk
    uint8_t chunk[16 * 128 * 16];

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

    free(cmd);
    return 0;
}