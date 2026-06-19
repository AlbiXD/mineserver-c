#include "../includes/bytes.h"

uint64_t BYTES_Ntoll(uint8_t *buffer)
{

    uint64_t x = 0;

    for (int i = 0; i < 8; i++)
    {
        x = (x << 8) | buffer[i];
    }

    return x;
}