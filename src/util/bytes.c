#include "../includes/bytes.h"

uint64_t BYTES_Ntohll(uint8_t *buffer)
{

    uint64_t x = 0;

    for (int i = 0; i < 8; i++)
    {
        x = (x << 8) | buffer[i];
    }

    return x;
}

double BYTES_ReadDouble(uint8_t *buffer, int *off)
{
    double tmp = 0;
    uint64_t raw = BYTES_Ntohll(buffer + *off);
    memcpy(&tmp, &raw, sizeof(double));
    *off += 8;
    return tmp;
}