#ifndef PACKET_H
#define PACKET_H

#include "client.h"
#include <unistd.h>

int packet_handler(Client *c);
int handle_handshake(Client *c, char* buf);
int handle_login(Client *c, char *buf);
int handle_pos(Client *c, char* buf);
#endif