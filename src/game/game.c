#include "../includes/game.h"
#include "../includes/packet.h"
#include "../includes/client.h"
int GAME_CommandHandler(command_t *cmd)
{

    printf("GAME HANDLER\n");

    int command_id = cmd->id;

    switch (command_id)
    {
    case HANDSHAKE:
    {
        uint8_t payload[] = {0x02, 0x00, 0x01, 0x00, 0x2D};

        write(cmd->sender->client_fd, payload, sizeof(payload));
        printf("Wrote handshake packet\n");
        return 0;
    }
    case LOGIN:
    {
        printf("LOGIN\n");
        uint8_t payload[] = {
            0x01,

            0x00, 0x00, 0x05, 0x12,

            0x00, 0x00,

            0x0D, 0x7C, 0x58, 0xD8, 0x8C, 0x4A, 0x91, 0xA,

            0x00};

        write(cmd->sender->client_fd, payload, 16);
        printf("Wrote login packet\n");
        return 0;
    }
    }

    return 0;
}
