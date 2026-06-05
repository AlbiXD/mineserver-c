#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <errno.h>
#include "config.h"

#define BACKLOG 128

typedef struct server server;

/*
Returns listening socket
*/
int create_listening_socket(server* server);

#endif