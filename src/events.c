#include "./includes/server.h"
#include "./includes/config.h"
#include "./includes/client.h"
#include "./includes/events.h"

int handle_accept_event(server *srv)
{
    struct sockaddr_in client_address;
    socklen_t client_addrlen = sizeof(client_address);

    for (;;)
    {
        memset(&client_address, 0, client_addrlen);
        int client_fd = accept(srv->server_fd, (struct sockaddr *)&client_address, &client_addrlen);
        printf("cfd=%d\n", client_fd);
        if (client_fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return 0;
            perror("accept");
            return -1;
        }

        printf("Client connected\n");
        int idx = check_for_open_slots(srv->clients, srv->max_players);
        printf("idx = %d\n", idx);

        // Terminate gracefully if no slots are open
        if (idx < 0)
        {
            reject_client(client_fd);
            continue;
        }

        add_client(srv, client_fd, client_address, idx);
    }
}

void handle_disconnect_event(client *cl)
{
    close(cl->client_fd);
}

int handle_read_event(client *cl)
{
    char buffer[CLIENT_BUFFER_SIZE];
    ssize_t bytes_read = 0;

    int f = read(cl->client_fd, buffer, CLIENT_BUFFER_SIZE);

    if (f <= 0)
    {
        printf("Client is hanging up\n");
        handle_disconnect_event(cl);
        return -1;
    }
    return 0;
}

void handle_events(server *srv)
{
    struct pollfd *pfd = srv->pfd_list;
    int n_pfd = srv->max_players + 1;

    // client connection
    if (pfd[0].revents & POLLIN)
    {
        handle_accept_event(srv);
        printf("Server pollfd\n");
    }

    // client I/O events n_pfd is 11
    // 0-10 0 is server socket, 1-10 are clients
    for (int i = 1; i < n_pfd; i++)
    {
        int ci = i - 1;

        // if(!srv->clients[i-1].is_used) continue;

        if (pfd[i].revents & (POLLHUP | POLLNVAL))
        {
            pfd[i].fd = -1;
            printf("SERVER: Client is quitting?\n");
        }

        if (pfd[i].revents & POLLIN)
        {
            // Handle client data
            printf("SERVER: Received data from client on fd %d\n", pfd[i].fd);
            if (handle_read_event(&srv->clients[ci]) < -1)
            {
                handle_disconnect_event(&srv->clients[ci]);
            }
        }
    }
}
