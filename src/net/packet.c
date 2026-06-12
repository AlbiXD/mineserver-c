// This file will be responsible for assembling a packet
#include "../includes/packet.h"

// int packet_parser(client* cl, client_packet packet){

// }

const packet_meta_t PLTB[ENUM_LENGTH] = {
    [LOGIN] = {LOGIN, 5, 4, 1},
    [HANDSHAKE] = {HANDSHAKE, 3, 2, 1}};

int PKT_Assemble(client *cl)
{
    packet_t packet;

    uint8_t *client_buffer = cl->client_buffer;
    uint8_t *packet_ptr = client_buffer + cl->packet_len;
    int size = 0;
    size_t bytes_read = cl->bytes_read;
    packet_status_t rval = PACKET_OK;

    printf("bytes_read=%d\n", bytes_read);

    while (1)
    {

        packet_ptr += size;
        cl->packet_len = size;

        if ((size_t)(packet_ptr - client_buffer) >= bytes_read)
        {
            return NEED_DATA;
        }

        size = 0;

        printf("packet_id=%d\n", *packet_ptr);
        // Array lookup table
        if ((*packet_ptr) > ENUM_LENGTH)
        {
            printf("Unknown Packet Type");
            return PACKET_ERROR;
        }

        if ((size = PKT_Length(client_buffer, packet_ptr, &cl->bytes_read, *packet_ptr)) < 0)
            return size;

        PKT_Init(&packet, *packet_ptr, packet_ptr, size);

        rval = PKT_Parser(&packet);
        PKT_Dispatcher(cl, *packet_ptr);
    }

    return rval;
}

void PKT_Dispatcher(client *cl, packet_id_t id)
{
    switch (id)
    {
    case HANDSHAKE:
    {
        printf("Handling handshake\n");
        unsigned char pkt_offline[] = {
            0x02,
            0x00, 0x01,
            0x00, 0x2D};

        write(cl->client_fd, pkt_offline, 5);
        break;
    }
    case LOGIN:
        printf("Login Packet\n");
        break;
    default:
        return;
    }

    return;
}

int PKT_Parser(packet_t *packet)
{
    return PACKET_OK;
}

int PKT_Init(packet_t *packet, packet_id_t id, uint8_t *payload, size_t packet_length)
{
    packet->id = id;
    packet->payload = payload;
    packet->packet_length = packet_length;
    return PACKET_OK;
}

int PKT_Length(uint8_t *client_buffer, uint8_t *packet_pointer, size_t *bytes_read_ptr, packet_id_t packet_id)
{

    if (packet_id == 0 || packet_id > 0x2)
    {
        printf("Packet Not Finished Yet\n");
        return PACKET_ERROR;
    }
    size_t bytes_read = *bytes_read_ptr;
    size_t offset = packet_pointer - client_buffer;
    size_t remaining_bytes = bytes_read - offset;
    int size = PLTB[packet_id].minSize;
    printf("offset=%td, size=%d\n", offset, size); // We have consumed all bytes

    int r = 0;

    // Do we have just enough information to get the dynamic size?
    if ((r = PKT_LengthCheck(offset, remaining_bytes, size)) == BUFFER_CONSUMED)
    {
        memmove(client_buffer, packet_pointer, remaining_bytes + 1);
        *bytes_read_ptr = remaining_bytes;
        return PACKET_INCOMPLETE;
    }
    else if (r == PACKET_INCOMPLETE)
        return PACKET_INCOMPLETE; // need to change the 3 into enumerated type

    size = packet_pointer[PLTB[packet_id].sizeOffset] * 2 + size;

    // Do we have the whole packet?
    if ((r = PKT_LengthCheck(offset, remaining_bytes, size)) == BUFFER_CONSUMED)
    {
        memmove(client_buffer, packet_pointer, remaining_bytes + 1);
        *bytes_read_ptr = remaining_bytes;
        return PACKET_INCOMPLETE;
    }
    else if (r == PACKET_INCOMPLETE)
        return PACKET_INCOMPLETE; // need to change the 3 into enumerated type

    printf("We have the full packet %02x, size=%d\n", packet_id, size);
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