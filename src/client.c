#include "./includes/client.h"
#include <sys/socket.h>

void init_client_list(client *clients, int max_players)
{
    for (int i = 0; i < max_players; i++)
    {
        clients[i].is_used = 0;
    }
}

int handle_client_connect(server *srv)
{
    for (;;)
    {
        int client_fd = accept(srv->server_fd, NULL, NULL);

        if (client_fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return 0;
            perror("accept");
            return -1;
        }
    }
}