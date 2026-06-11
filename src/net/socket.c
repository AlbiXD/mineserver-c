#include "../includes/socket.h"
#include "../includes/server.h"

int SOCKET_CreateListening(server *srv)
{

    int sfd = 0;
    const config *cfg = srv->cfg;

    // Server address
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
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    // Make socket non blocking
    if (fcntl(sfd, F_SETFL, O_NONBLOCK) < 0)
        return -1;

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

    return sfd;
}