#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int handshake_length(uint8_t *buffer, size_t bytes_read);

#endif 