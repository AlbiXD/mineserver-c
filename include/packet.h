#ifndef PACKET_H
#define PACKET_H

#include "client.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "util.h"
int packet_handler(Client *c);
int handle_handshake(Client *c, unsigned char* buf, int n);
int handle_login(Client *c, unsigned char *buf);
int handle_pos(Client *c, unsigned char* buf);
int handle_chat(Client *c, unsigned char *buf, int n);
#endif