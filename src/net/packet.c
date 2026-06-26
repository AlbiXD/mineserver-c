// This file will be responsible for assembling a packet
#include "../includes/packet.h"
#include "../includes/game.h"
#include "../includes/bytes.h"
#include "../includes/events.h"
// int packet_parser(client* cl, client_packet packet){

// }

const packet_meta_t PLTB[256] = {
    [LOGIN] = {LOGIN, 16, 6, 1},
    [HANDSHAKE] = {HANDSHAKE, 3, 2, 1},
    [POSITION] = {POSITION, 34, 0, 0},
    [LOOK] = {LOOK, 10, 0, 0},
    [POSITION_AND_LOOK] = {POSITION_AND_LOOK, 42, 0, 0},
    [DISCONNECT] = {DISCONNECT, 3, 2, 1},
    [KEEP_ALIVE] = {KEEP_ALIVE, 1, 0, 0}};

int PKT_Assemble(client *cl, cmd_queue *queue)
{
    packet_t packet;

    uint8_t *client_buffer = cl->net.buffer;
    uint8_t *packet_ptr = client_buffer + cl->net.packet_len;
    int size = 0;
    size_t bytes_read = cl->net.bytes_read;
    packet_status_t rval = PACKET_OK;

    while (1)
    {
        packet_ptr += size;
        // printf("%lu", (size_t)(packet_ptr - client_buffer));

        if ((size_t)(packet_ptr - client_buffer) >= bytes_read)
        {
            // printf("data = %lu\n", (size_t)(packet_ptr - client_buffer));
            // printf("data_bytes_read=%lu\n", bytes_read);
            // printf("Need more data\n");
            return NEED_DATA;
        }

        // printf("packet_ptr=%p, size2=%d\n", packet_ptr, size);
        size = 0;

        // printf("packet_id=%ld\n", *packet_ptr);
        if (PLTB[*packet_ptr].id == 0)
        {
            printf("Unknown Packet Type\n");
            return PACKET_UNSUPPORTED;
        }

        if ((size = PKT_Length(client_buffer, packet_ptr, &cl->net.bytes_read, *packet_ptr)) < 0)
            return size;

        cl->net.packet_len += size;
        // printf("%d\n", cl->net.packet_len);
        PKT_Init(&packet, *packet_ptr, packet_ptr, size);

        if ((rval = PKT_Parser(&packet, cl, queue)) == PACKET_DISCONNECT)
            return PACKET_DISCONNECT;
    }

    return rval;
}

/*
    Packet Validation -> sends packet to command constructor?
*/
int PKT_Parser(packet_t *packet, client *sender, cmd_queue *queue)
{
    game_command_t *cmd = malloc(sizeof(game_command_t));
    packet_id_t id = packet->id;

    switch (id)
    {

    case HANDSHAKE:
    {
        int idx = 0;
        for (size_t i = 4; i < packet->packet_length; i += 2)
        {
            cmd->payload.handshake.username[idx++] = packet->payload[i];
        }

        cmd->payload.handshake.username[idx] = '\0';
        cmd->id = HANDSHAKE;
        cmd->sender = sender;
        printf("Parsing Handshake\n");
        return GAME_Handshake(cmd);
    }
    case LOGIN:
    {
        cmd->payload.login.username[0] = '\0';
        cmd->id = LOGIN;
        cmd->sender = sender;
        printf("Login\n");
        return GAME_Login(cmd);
    }
    case LOOK:
    {
        printf("LOOK\n");
        break;
    }

    case POSITION:
    {

        int off = 1;

        double X = BYTES_ReadDouble(packet->payload, &off);
        double Y = BYTES_ReadDouble(packet->payload, &off);
        double stance = BYTES_ReadDouble(packet->payload, &off);
        double Z = BYTES_ReadDouble(packet->payload, &off);
        uint8_t on_ground = *(packet->payload + off + 1);

        cmd->id = POSITION;
        cmd->payload.position = (cmd_client_position_t){X, Y, stance, Z, on_ground};
        cmd->sender = sender;
        break;
    }
    case POSITION_AND_LOOK:
    {
        return 0;
    }
    case DISCONNECT:
    {
        return PACKET_DISCONNECT;
    }

    default:
    {
        printf("Unknown Packet Type Cannot Parse\n");
        return PACKET_UNSUPPORTED;
    }
    }

    CMDQ_Push(queue, cmd);

    return PACKET_OK;
}

int PKT_Init(packet_t *packet, packet_id_t id, uint8_t *payload, size_t packet_length)
{
    *packet = (packet_t){
        .id = id,
        .payload = payload,
        .packet_length = packet_length};
    return PACKET_OK;
}

int PKT_Length(uint8_t *client_buffer, uint8_t *packet_pointer, size_t *bytes_read_ptr, packet_id_t packet_id)
{

    // if (packet_id == 0 || packet_id > 0x2)
    // {
    //     printf("Packet Not Finished Yet\n");
    //     return PACKET_UNSUPPORTED;
    // }
    size_t bytes_read = *bytes_read_ptr;
    size_t offset = packet_pointer - client_buffer;
    size_t remaining_bytes = bytes_read - offset;
    int size = PLTB[packet_id].minSize;
    // printf("offset=%td, size=%d\n", offset, size); // We have consumed all bytes
    // printf("remaining_bytes=%d, bytes_read=%d\n", remaining_bytes, bytes_read);

    int r = 0;

    // Do we have just enough information to get the dynamic size?
    if ((r = PKT_LengthCheck(offset, remaining_bytes, size)) == BUFFER_CONSUMED)
    {
        memmove(client_buffer, packet_pointer, remaining_bytes + 1);
        *bytes_read_ptr = remaining_bytes;
        return PACKET_INCOMPLETE;
    }
    else if (r == PACKET_INCOMPLETE)
    {

        return PACKET_INCOMPLETE; // need to change the 3 into enumerated type
    }

    if (PLTB[packet_id].isSized)
    {
        size = packet_pointer[PLTB[packet_id].sizeOffset] * 2 + size;
    }
    else
    {
        size = PLTB[packet_id].minSize;
    }

    // printf("size=%d\n", size);
    // Do we have the whole packet?
    if ((r = PKT_LengthCheck(offset, remaining_bytes, size)) == BUFFER_CONSUMED)
    {
        memmove(client_buffer, packet_pointer, remaining_bytes + 1);
        *bytes_read_ptr = remaining_bytes;
        return PACKET_INCOMPLETE;
    }
    else if (r == PACKET_INCOMPLETE)
        return PACKET_INCOMPLETE; // need to change the 3 into enumerated type

    // printf("We have the full packet %02x, size=%d\n", packet_id, size);
    return size;
}

void PKT_Mmove(uint8_t *client_buffer, uint8_t *packet, size_t packet_len)
{
    memmove(client_buffer, packet, packet_len);
}

int PKT_LengthCheck(size_t offset, size_t remaining_bytes, size_t size)
{
    // Do we have the minimum header
    //  Do I have minimum bytes for header?
    if (offset + size > BUFFER_LENGTH) // Do we have enough bytes?
        return BUFFER_CONSUMED;

    // Is my data valid beyond this pointer? 0A 0B 0C 02 00 06 5-4 == 1
    // offset = 4
    // bytes_read = 5
    // 5-4 == 1 meaning there is one byte that is valid
    if (remaining_bytes < size) // if so we need 2 bytes ahead of this that are valid
        return PACKET_INCOMPLETE;

    return PACKET_OK;
}