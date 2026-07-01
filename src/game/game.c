#include "../includes/game.h"
#include "../includes/packet.h"
#include "../includes/client.h"
#include "../includes/cmd_queue.h"
#include "../includes/server.h"
#include <zlib.h>

int GAME_Tick(cmd_queue *queue, server *srv)
{

    game_command_t *cmd;
    while ((cmd = CMDQ_Pop(queue)) != NULL)
    {
        int cmd_id = cmd->id;
        client *sender = cmd->sender;
        switch (cmd_id)
        {
        case POSITION:
        {
            player_location_t dL = {
                .position = {
                    .X = cmd->payload.position.X,
                    .Y = cmd->payload.position.Y,
                    .Z = cmd->payload.position.Z,
                    .stance = cmd->payload.position.stance},
                .on_ground = cmd->payload.position.onGround,
                .look = sender->player.loc.look};

            double dX = sender->player.loc.position.X;
            double dY = sender->player.loc.position.Y;
            double dZ = sender->player.loc.position.Z;
            double dStance = sender->player.loc.position.stance;

            sender->player.loc = dL;

            int dx = (int)(sender->player.loc.position.X * 32) - (int)(dX * 32);
            int dy = (int)(sender->player.loc.position.Y * 32) - (int)(dY * 32);
            int dz = (int)(sender->player.loc.position.Z * 32) - (int)(dZ * 32);

            // printf("dx=%d, dy=%d, dz=%d\n", dx, dy, dz);
            // Client seems to be falling through the block? check if we are touching ground?

            int32_t eid = htonl(sender->player.entity_id);

            uint8_t pkt_entity_move[8];
            pkt_entity_move[0] = 0x1F;
            memmove(pkt_entity_move + 1, &eid, sizeof(int32_t));
            pkt_entity_move[5] = (int8_t)dx;
            pkt_entity_move[6] = (int8_t)dy;
            pkt_entity_move[7] = (int8_t)dz;

            for (int i = 0; i < srv->max_players; i++)
            {
                if (!srv->clients[i].is_used)
                    continue;

                if (srv->clients[i].idx == sender->idx)
                    continue;

                printf("entity_moving = %d\n", sender->player.entity_id);
                write(srv->clients[i].net.fd, pkt_entity_move, 8);
                printf("Wrote to other entity = %d\n", srv->clients->player.entity_id);
            }
            break;
        }
        }
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
int GAME_Login(server *srv, game_command_t *cmd)
{
    client *sender = cmd->sender;
    int fd = sender->net.fd;

    if (sender->state != CLIENT_LOGIN)
        return STATE_ERROR;

    printf("LOGIN\n");

    sender->player.entity_id = global_id++;
    printf("assigned entity=%d global_id now=%d\n",
           sender->player.entity_id,
           global_id);

    uint8_t payload[] = {
        0x01,

        // entity id placeholder
        0x00, 0x00, 0x00, 0x00,

        0x00, 0x00,

        0x0D, 0x7C, 0x58, 0xD8, 0x8C, 0x4A, 0x91, 0x0A,

        0x00};

    int32_t eid_net = htonl(sender->player.entity_id);

    memcpy(&payload[1], &eid_net, sizeof(eid_net));

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

    sender->player.loc.position = (player_position_t){
        .X = 14.0,
        .Y = 128.0,
        .Z = 12.0};

    sender->player.loc.position.stance = sender->player.loc.position.Y + 1.62;

    sender->player.loc.on_ground = 1;

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
        0x01};

    write(fd, pkt_poslook, sizeof(pkt_poslook));

    uint8_t *pkt = malloc(60);

    // Draw player on initial log in
    for (int i = 0; i < srv->max_players; i++)
    {
        if (!srv->clients[i].is_used) // Not used
            continue;

        if (&srv->clients[i] == sender) // Make sure not to write the current person i guess
            continue;

        uint8_t name[8] = "Client";
        // 0-7
        name[6] = 0x30 + sender->player.entity_id;
        int pkt_len = GAME_DrawEntity(pkt, name, sizeof(name), sender, &pkt_len);

        write(srv->clients[i].net.fd, pkt, pkt_len);

        name[6] = 0x30 + srv->clients->player.entity_id;
        pkt_len = GAME_DrawEntity(pkt, name, sizeof(name), &srv->clients[i], &pkt_len);
        write(sender->net.fd, pkt, pkt_len);
    }

    free(pkt);
    free(cmd);
    return 0;
}

int GAME_DrawEntity(uint8_t *pkt_named_entity, uint8_t *name, int16_t name_len, client *sender, int *pkt_len)
{
    int pkt_size = (name_len * 2) + 23;

    int32_t eid = htonl(sender->player.entity_id);
    int32_t x = htonl((int32_t)(sender->player.loc.position.X * 32.0));
    int32_t y = htonl((int32_t)(sender->player.loc.position.Y * 32.0));
    int32_t z = htonl((int32_t)(sender->player.loc.position.Z * 32.0));
    int len = name_len;
    name_len = htons(name_len);

    int16_t item = htons(0);

    int off = 0;

    pkt_named_entity[off++] = 0x14;

    memcpy(pkt_named_entity + off, &eid, 4);
    off += 4;

    memcpy(pkt_named_entity + off, &name_len, 2);
    off += 2;

    int idx = 0;
    for (int i = 0; i < len * 2; i++)
    {
        if (!(i % 2))
        {
            pkt_named_entity[off++] = 0x00;
            continue;
        }

        pkt_named_entity[off++] = name[idx++];
    }

    memcpy(pkt_named_entity + off, &x, 4);
    off += 4;

    memcpy(pkt_named_entity + off, &y, 4);
    off += 4;

    memcpy(pkt_named_entity + off, &z, 4);
    off += 4;

    pkt_named_entity[off++] = 0; // rotation
    pkt_named_entity[off++] = 0; // pitch

    memcpy(pkt_named_entity + off, &item, 2);
    off += 2;

    *pkt_len = pkt_size;
    return pkt_size;
}