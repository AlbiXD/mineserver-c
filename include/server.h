#ifndef SERVER_H
#define SERVER_H
#define _GNU_SOURCE

#include "cfg.h"
#include "packet.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>

typedef struct{
    int server_fd;
    struct sockaddr_in server_addr;
    Config* cfg;
    int port;
    int max_players;
    int online_players;
} Server;
/*


*/

Server * init_server(Config* cfg);

void disconnect_handler(struct pollfd *pfd, int* online, Client* c, int max_clients);
void handle_new_connections(Server *server, struct pollfd* pfd,  Client* clients, int pfd_n, int max_clients);
void start_server(Server *server);

void server_stop(Server *server);

#endif