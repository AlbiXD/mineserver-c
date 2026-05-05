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

    printf("SERVER: Initializing server...\n");

    // Construct the IP
    memset(&srv->server_addr, 0, sizeof(srv->server_addr));

    srv->server_addr.sin_family = AF_INET; // Set Family

    srv->server_addr.sin_port = htons(cfg->PORT); // Set Port

    if (strcmp(cfg->ip_address, "localhost") == 0)

        srv->server_addr.sin_addr.s_addr = INADDR_ANY;
     else
        srv->server_addr.sin_addr.s_addr = inet_addr(cfg->ip_address);


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