#ifndef BYTES_H
#define BYTES_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
uint64_t BYTES_Ntohll(uint8_t *buffer);
double BYTES_ReadDouble(uint8_t *buffer, int *off);
#endif