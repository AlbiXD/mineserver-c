#ifndef PACKET_H
#define PACKET_H

#include "client.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "util.h"
int packet_assembler(Client *c, unsigned char* instream, unsigned char* packet_stream, int n);
int packet_handler(Client *c);
int handle_handshake(Client *c, unsigned char* buf);
int handle_login(Client *c, unsigned char *buf);
int handle_pos(Client *c, unsigned char* buf);
int handle_chat(Client *c, unsigned char *buf);
#endif