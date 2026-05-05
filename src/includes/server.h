#ifndef SERVER_H
#define SERVER_H

#include "config.h"
#include <sys/socket.h>
#define BACKLOG 128

typedef struct {
    int server_fd;          // listening socket
    struct sockaddr_in server_addr;
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
 * or a error occurs.
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