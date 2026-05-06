#ifndef SERVER_H
#define SERVER_H
#include "config.h"
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

#define BACKLOG 128

typedef struct client client;

typedef struct server {
    int server_fd;                  /* listening socket */
    struct sockaddr_in server_addr; /* address the server is bound to */
    client *clients;                /* array of client slots, length max_players */
    int max_players;                /* size of the clients array */
    int pollfd_index;               /* number of active entries in pfd_list */
    struct pollfd *pfd_list;        /* poll() fd array; index 0 is the listener */
    const config *cfg;              /* server configuration, owned by caller */
} server;

/*
 * Prepare the server: create socket, bind, listen.
 * Does NOT accept connections yet.
 *
 * Returns 0 on success, -1 on failure.
 */
int init_server(server *srv, const config *cfg);

/*
 * Enter the accept loop. Blocks until stop_server() is called
 * or an error occurs.
 *
 * Returns 0 on clean shutdown, -1 on error.
 */
int start_server(server *srv);

/*
 * Signal the accept loop to exit.
 */
void stop_server(server *srv);

/*
 * Release all resources held by server.
 */
void destroy_server(server *srv);

#endif