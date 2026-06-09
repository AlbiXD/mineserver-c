#include "../includes/protocol.h"

int handshake_length(uint8_t *buffer, size_t bytes_read){

    int size = 0;
    if(bytes_read < 3) return -1;
    size = buffer[2];
    if(bytes_read < size) return -1;

    printf("handling hanshake %d\n", size);

    return size;
}