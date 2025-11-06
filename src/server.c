#include "../include/server.h"
#include "../include/cfg.h"
#include "../include/client.h"
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

Server *init_server(Config *cfg)
{
    Server *server = (Server *)malloc(sizeof(Server));
    memset(&server->server_addr, 0, sizeof(server->server_addr));

    server->server_addr.sin_family = AF_INET;
    if (strcmp(cfg->host, "localhost") == 0)
    {
        server->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        printf("Localhost\n");
    }
    else
    {
        if (!inet_aton(cfg->host, &server->server_addr.sin_addr))
        {
            perror("Invalid IP address");
            exit(1);
        }
    }

    server->server_addr.sin_port = htons(cfg->port);
    server->port = cfg->port;
    server->cfg = cfg;
    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server->online_players = 0;
    server->max_players = cfg->max_players;
    int flags = fcntl(server->server_fd, F_GETFL, 0);
    fcntl(server->server_fd, F_SETFL, flags | O_NONBLOCK); // Makes it non blocking

    return server;
}

void start_server(Server *server)
{
    int pfd_n = server->max_players + 1;
    int max_clients = server->max_players;
    int *online = &server->online_players;

    struct pollfd pfd[pfd_n];
    Client clients[max_clients];

    init_tables(pfd, clients, pfd_n, max_clients); // Initialize tables

    pfd[0].fd = server->server_fd;
    pfd[0].events = POLLIN;

    printf("Starting Server on IP: %s:%d\n", server->cfg->host, server->port);
    if (bind(server->server_fd, (struct sockaddr *)&server->server_addr, sizeof(server->server_addr)) < 0)
    {
        fprintf(stderr, "Failed to bind on port %d: %s\n", server->port, strerror(errno));
        server_stop(server);
    }
    if (listen(server->server_fd, 5) < 0)
    {
        fprintf(stderr, "Failed to listen on port %d: %s\n", server->port, strerror(errno));
        server_stop(server);
    }

    printf("Server is now online\n");

    for (;;)
    {
        int p = poll(pfd, pfd_n, -1);

        if (*online) // If no player is online why bother
        {
            for (int i = 1; i < pfd_n - 1; i++)
            { // Handle client stuff
                if (pfd[i].fd == -1)
                    continue;

                if (pfd[i].revents == POLLIN)
                {
                    int f = packet_dump(pfd[i].fd);//I just wanna see what they wrote
                    printf("Client wrote %d bytes\n", f);
                }
                pfd[i].revents = 0;
            }
        }
        if (pfd[0].revents & POLLIN)
        { // Player connection handle
            for (;;)
            {
                struct sockaddr_in client_addr;
                socklen_t cli_addr_len = sizeof(client_addr);
                memset(&client_addr, 0, sizeof(client_addr));

                int cfd = accept(server->server_fd, (struct sockaddr *)&client_addr, &cli_addr_len); // Client connection

                if (cfd < 0)
                { // If no more clients in the kernel queue
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                    perror("accept");
                    break;
                }
                if (server->online_players < server->max_players)
                { // If server is not full assign new player
                    printf("Accepted user!\n");
                    add_client(pfd, clients, pfd_n, max_clients, cfd, &client_addr); // Adds client to client list and assigns client to pfd
                    (*online)++;
                }
                else
                { // If server is full close gracefully
                    printf("Server is full!\n");
                    close(cfd);
                }
            }
            pfd[0].revents = 0;
        }
    }

    return;
}

void server_stop(Server *server)
{
    close(server->server_fd);
    free(server);
    exit(1);
}
