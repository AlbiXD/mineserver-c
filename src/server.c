#include "includes/server.h"
#include "includes/config.h"
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>

int init_server(server *srv, const config *cfg)
{

    int sfd;
    struct sockaddr *addr = (struct sockaddr *)&srv->server_addr;
    struct sockaddr_in *addr_in = &srv->server_addr;

    printf("SERVER: Initializing server...\n");

    // Construct the IP
    memset(addr_in, 0, sizeof(*addr_in));

    addr_in->sin_family = AF_INET; // Set Family
    addr_in->sin_port = htons(cfg->PORT); // Set Port

    if (strcmp(cfg->ip_address, "localhost") == 0)
        addr_in->sin_addr.s_addr = INADDR_ANY;
    else
        addr_in->sin_addr.s_addr = inet_addr(cfg->ip_address);

    printf("SERVER: Starting on IP %s\n", cfg->ip_address);
    printf("SERVER: Running on PORT %hu\n", cfg->PORT);

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

    printf("SERVER: Server initialized!\n");

    return 0;
}