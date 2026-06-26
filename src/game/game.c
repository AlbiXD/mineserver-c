#include "../includes/game.h"
#include "../includes/packet.h"
#include "../includes/client.h"
#include "../includes/cmd_queue.h"
#include <zlib.h>
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

    uint8_t pkt_prechunk[] = {
        0x32,

        // X
        0x00, 0x00, 0x00, 0x00,

        // Y
        0x00, 0x00, 0x00, 0x00,

        0x01};

    write(fd, pkt_prechunk, sizeof(pkt_prechunk));
    // send chunk
    uint8_t blocks[16 * 128 * 16];
    uint8_t metadata[(16 * 128 * 16) / 2];
    uint8_t blocklight[(16 * 128 * 16) / 2];
    uint8_t skylight[(16 * 128 * 16) / 2];

    memset(blocks, 2, sizeof(blocks)); // Every block = stone (ID 1)
    memset(metadata, 0, sizeof(metadata));
    memset(blocklight, 0, sizeof(blocklight));
    memset(skylight, 0xFF, sizeof(skylight));

    uint8_t chunk[81920];

    memcpy(chunk, blocks, 32768);
    memcpy(chunk + 32768, metadata, 16384);
    memcpy(chunk + 32768 + 16384, blocklight, 16384);
    memcpy(chunk + 32768 + 16384 + 16384, skylight, 16384);

    uLong source_size = 81920;

    uLongf max_compressed = compressBound(source_size);

    uint8_t *compressed = malloc(max_compressed);

    compress(compressed, &max_compressed, chunk, sizeof(chunk));

    uint8_t *pkt_chunk = malloc(max_compressed + 18);
    // 0-9
    // Set x,y,z as 0,0,0
    int index = 0;
    pkt_chunk[index++] = 0x33;
    // 1+10 = 11= 1,2,3,4,5,6,7,8,9,10,11
    memset(pkt_chunk + index, 0, 10);
    index += 10;
    pkt_chunk[index++] = 0x0F;
    pkt_chunk[index++] = 0x7F;
    pkt_chunk[index++] = 0x0F;

    typedef union convert
    {
        uint8_t byte[4];
        uint32_t compressed;
    } convert;

    convert test = {.compressed = max_compressed};

    for (int i = 3; i >= 0; i--)
    {
        pkt_chunk[index++] = test.byte[i];
    }

    memmove(pkt_chunk + index, compressed, max_compressed);

    write(fd, pkt_chunk, max_compressed + 18);

    unsigned char pkt_poslook[] = {
        0x0D,

        // X
        0x40, 0x2C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,

        // Y
        0x40, 0x60, 0x60, 0x00,
        0x00, 0x00, 0x00, 0x00,

        // Stance
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,

        // Z
        0x40, 0x28, 0x00, 0x00,
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