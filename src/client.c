#include "./includes/server.h"
#include "./includes/client.h"
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
void init_client_list(server* srv)
{
    for (int i = 0; i < srv->max_players; i++)
    {
        srv->clients[i].is_used = 0;
        srv->pfd_list[i+1].fd = -1;
        srv->pfd_list[i+1].events = 0;
        srv->pfd_list[i+1].revents = 0;

    }
}

int check_for_open_slots(client *clients, int max_players)
{

    for (int i = 0; i < max_players; i++)
        if (clients[i].is_used == 0)
            return i;

    return -1;
}

void add_client(server *srv, int client_fd, struct sockaddr_in client_address, int idx)
{
    srv->clients[idx].client_addr = client_address;
    srv->clients[idx].client_fd = client_fd;
    srv->clients[idx].is_used = 1;
    srv->clients[idx].pfd_idx = idx + 1;
    srv->clients[idx].idx = idx;

    srv->pfd_list[idx + 1].fd = client_fd;
    srv->pfd_list[idx + 1].events = POLLIN;
}

void reject_client(int client_fd)
{
    close(client_fd);
}

int handle_client_connect(server *srv)
{
    struct sockaddr_in client_address;
    socklen_t client_addrlen = sizeof(client_address);

    for (;;)
    {
        memset(&client_address, 0, client_addrlen);
        int client_fd = accept(srv->server_fd, (struct sockaddr *)&client_address, &client_addrlen);

        if (client_fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return 0;
            perror("accept");
            return -1;
        }

        printf("Client connected\n");
        int idx = check_for_open_slots(srv->clients, srv->max_players);

        // Terminate gracefully
        if (idx < 0)
        {
            reject_client(client_fd);
            continue;
        }

        add_client(srv, client_fd, client_address, idx);
    }
}