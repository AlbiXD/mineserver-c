#include "includes/server.h"
#include "includes/config.h"
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <errno.h>
#include "./includes/client.h"


int init_server(server *srv, const config *cfg)
{

    int sfd;
    srv->cfg = cfg;

    // Initialize the client list
    srv->clients = malloc(sizeof(client) * cfg->max_players);
    if (!srv->clients)
    {
        perror("malloc");
        return -1;
    }
    srv->max_players = cfg->max_players;
    init_client_list(srv->clients, cfg->max_players);

    struct sockaddr *addr = (struct sockaddr *)&srv->server_addr;
    struct sockaddr_in *addr_in = &srv->server_addr;
    printf("SERVER: Initializing server...\n");

    // Construct the IP
    memset(addr_in, 0, sizeof(*addr_in));
    addr_in->sin_family = AF_INET;        // Set Family
    addr_in->sin_port = htons(cfg->port); // Set Port
    if (strcmp(cfg->ip_address, "localhost") == 0)
        addr_in->sin_addr.s_addr = INADDR_ANY;
    else
        addr_in->sin_addr.s_addr = inet_addr(cfg->ip_address);

    // Create socket
    srv->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sfd = srv->server_fd;


    // Make socket non blocking
    fcntl(sfd, F_SETFL, O_NONBLOCK);

    // Bind
    if (bind(sfd, addr, sizeof(*addr)) < 0)
    {
        perror("SERVER");
        return -1;
    }

    // Listen
    if (listen(sfd, BACKLOG) < 0)
    {
        perror("SERVER");
        return -1;
    }
    printf("SERVER: Starting on IP %s\n", cfg->ip_address);
    printf("SERVER: Running on PORT %hu\n", cfg->port);

    printf("SERVER: Server initialized!\n");

    return 0;
}

int start_server(server *srv)
{

    printf("SERVER: Server is now online and ready to accept clients\n");
    int n_pfd = srv->max_players + 1;

    struct pollfd pfd[n_pfd];

    srv->pfd_list = pfd;

    pfd[0].fd = srv->server_fd;
    pfd[0].events = POLLIN;
    pfd[0].revents = 0;

    while (1)
    {

        poll(pfd, n_pfd, -1);


        //client connection
        if (pfd[0].revents & POLLIN)
            handle_client_connect(srv);

    }

    return 0;
}

void stop_server(server *srv)
{

    // Need to free and close clients
    close(srv->server_fd); // Close Server
    return;
}