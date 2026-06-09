#include "../includes/server.h"
#include "../includes/config.h"
#include "../includes/client.h"
#include "../includes/events.h"

int handle_accept_event(server *srv)
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
        if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
            return -1;
        printf("Client connected\n");
        int idx = check_for_open_slots(srv->clients, srv->max_players);

        // Terminate gracefully if no slots are open
        if (idx < 0)
        {
            reject_client(client_fd);
            continue;
        }

        add_client(srv, client_fd, client_address, idx);
    }
}

void handle_disconnect_event(server *srv, client *cl)
{
    printf("Disconnect event\n");
    struct pollfd *pfd = srv->pfd_list;
    pfd[cl->pfd_idx].fd = -1;
    cl->bytes_read = 0;
    cl->is_used = 0;
    close(cl->client_fd);
}

int handle_read_event(client *cl)
{
    char *client_buffer = cl->client_buffer;

    size_t buflen = KB * MULTIPLE;
    size_t bytes_read = cl->bytes_read;

    int n = 0;
    uint8_t new_data = 0;

    int rval = 0;
    while (1)
    {

        if (bytes_read == buflen)
        {
            bytes_read = 0;
            cl->bytes_read = bytes_read;
            break;
        } // flush buffer?

        n = read(cl->client_fd, client_buffer + bytes_read, buflen - bytes_read); // Drain the read buffer until full then flush?

        if (n > 0)
        {
            new_data = 1;
            bytes_read += n;
            cl->bytes_read = bytes_read; // stores for later use
            printf("Reading...\n");
            continue;
        }

        if (n <= 0) // Will check if kernel has no more user bytes
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            return -1;
        }


    }
    printf("bytes_read=%lu\n", bytes_read);

    if (n == 0)
    {
        printf("Client is hanging up\n");
        return -1;
    }

    if (new_data)
        rval = packet_assembler(cl);

    return rval;
}

void handle_events(server *srv)
{
    struct pollfd *pfd = srv->pfd_list;
    int n_pfd = srv->max_players + 1;

    poll(pfd, n_pfd, -1);

    // client connection
    if (pfd[0].revents & POLLIN)
        handle_accept_event(srv);

    // client I/O events n_pfd is 11
    // 0-10 0 is server socket, 1-10 are clients
    for (int i = 1; i < n_pfd; i++)
    {
        if (pfd[i].fd == -1)
            continue;

        int ci = i - 1;
        client *cl = &srv->clients[ci];
        // if(!srv->clients[i-1].is_used) continue;

        if (pfd[i].revents & (POLLHUP | POLLNVAL))
        {
            pfd[i].fd = -1;
            printf("SERVER: Client is quitting?\n");
            handle_disconnect_event(srv, cl);
        }

        if (pfd[i].revents & POLLIN)
        {
            // Handle client data
            printf("SERVER: Received data from client on fd %d\n", pfd[i].fd);
            if (handle_read_event(cl) <= EOF)
                handle_disconnect_event(srv, cl);
        }
    }
}
