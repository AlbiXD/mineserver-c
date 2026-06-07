#include "../includes/server.h"
#include "../includes/config.h"
#include "../includes/client.h"
#include "../includes/events.h"

int init_server(server *srv, const config *cfg)
{

    srv->cfg = cfg;                                                         // Point to config in main
    srv->max_players = cfg->max_players;                                    // Server max players
    srv->clients = malloc(sizeof(client) * cfg->max_players);               // Initialize the client list
    srv->pfd_list = malloc(sizeof(struct pollfd) * (srv->max_players + 1)); // Initialize the pollfd list

    if ((srv->server_fd = create_listening_socket(srv)) < 0)
        return -1; // Remove line maybe

    if (!srv->clients)
    {
        perror("malloc");
        return -1;
    }

    if (!srv->pfd_list)
    {
        perror("malloc");
        return -1;
    }

    init_client_list(srv); // Initializes the client list + pollfd list

    printf("SERVER: Starting on IP %s\n", cfg->ip_address);
    printf("SERVER: Running on PORT %hu\n", cfg->port);
    printf("SERVER: Server initialized!\n");

    return 0;
}

int start_server(server *srv)
{

    printf("SERVER: Server is now online and ready to accept clients\n");
    struct pollfd *pfd = srv->pfd_list;

    // Listening socket / server socket poll
    pfd[0].fd = srv->server_fd;
    pfd[0].events = POLLIN | POLLHUP | POLLERR;
    pfd[0].revents = 0;

    for (;;) // Main server loop
    {
        handle_events(srv);
    }

    return 0;
}

void stop_server(server *srv)
{

    printf("SERVER: Shutting down server...\n");
    // Need to free and close clients
    close_all_clients(srv);
    close(srv->server_fd); // Close Server
    free(srv->pfd_list);
    free(srv->clients);
    return;
}