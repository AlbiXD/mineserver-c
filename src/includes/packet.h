#ifndef PACKET_H
#define PACKET_H

#include "client.h"
#include "protocol.h"

int packet_assembler(client * cl);
int handshake_packet(client * cl);


#endif