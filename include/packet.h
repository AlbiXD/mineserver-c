#ifndef PACKET_H
#define PACKET_H

#include "client.h"
#include <unistd.h>

int packet_handler(Client *c);
int handle_handshake(Client *c, unsigned char* buf);
int handle_login(Client *c, unsigned char *buf);
int handle_pos(Client *c, unsigned char* buf);
#endif