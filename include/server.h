#ifndef SERVER_H
#define SERVER_H

#include "cfg.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
typedef struct{
    int server_fd;
    struct sockaddr_in server_addr;
    Config* cfg;
    int port;
    int max_players;
} Server;
/*


*/

Server * init_server(Config* cfg);


void start_server(Server *server);

void server_stop(Server *server);



#endif