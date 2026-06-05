#include "./includes/server.h"
#include "./includes/client.h"

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
    srv->pfd_list[idx + 1].events = POLLIN | POLLHUP | POLLERR;
}

void reject_client(int client_fd)
{
    close(client_fd);
}

void close_all_clients(server *srv)
{
    for (int i = 0; i < srv->max_players; i++)
    {
        if (srv->clients[i].is_used)
        {
            close(srv->clients[i].client_fd);
            srv->clients[i].is_used = 0;
        }
    }
}
