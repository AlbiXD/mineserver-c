#include "../includes/server.h"
#include "../includes/config.h"
#include "../includes/client.h"
#include "../includes/events.h"

int NEVENT_Accept(server *srv)
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
        int idx = CL_OpenSlot(srv->clients, srv->max_players);

        // Terminate gracefully if no slots are open
        if (idx < 0)
        {
            CL_Reject(client_fd);
            continue;
        }

        CL_Add(srv, client_fd, client_address, idx);
    }
}

void NEVENT_Disconnect(server *srv, client *cl)
{
    printf("Disconnect event\n");
    struct pollfd *pfd = srv->pfd_list;
    cl->net.packet_len = 0;
    pfd[cl->net.pfd_idx].fd = -1;
    cl->net.bytes_read = 0;
    cl->is_used = 0;
    close(cl->net.fd);
}

int NEVENT_Read(client *cl)
{
    uint8_t *client_buffer = cl->net.buffer;

    size_t bytes_read = cl->net.bytes_read;

    int n = 0;
    uint8_t new_data = 0;
    int r = 0;
    int    rval = 0;
    while (1)
    {

        //980 -> 1024  //BUFFER FULL
        if (bytes_read == BUFFER_LENGTH)
        {
            printf("b_read=%d\n", bytes_read);
            memmove(client_buffer, client_buffer+BUFFER_LENGTH-r, r);
            bytes_read = r;
            printf("TRUE\n");
            cl->net.bytes_read = bytes_read;
            cl->net.packet_len = 0;
            break;
        } // flush buffer?

        n = read(cl->net.fd, client_buffer + bytes_read, BUFFER_LENGTH - bytes_read); // Drain the read buffer until full then flush?
    
        if (n > 0)
        {
            r = n;
            new_data = 1;
            bytes_read += n;
            cl->net.bytes_read = bytes_read; // stores for later use
            // for(size_t i = 0; i < n; i++){
            //     printf("%02x", client_buffer[i]);
            // }
            // printf("\n");
            continue;
        }

        if (n <= 0) // Will check if kernel has no more user bytes
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            return -1;
        }
    }
    // printf("bytes_read=%lu\n", bytes_read);

    if (n == 0)
    {
        printf("Client is hanging up\n");
        return -3;
    }

    if (new_data)
    {
        // printf("Assembling packet\n");
        rval = PKT_Assemble(cl);
    }
    if (rval == BUFFER_CONSUMED)
    {
        cl->net.bytes_read = 0;
    }
    return rval;
}

void NEVENT_Handle(server *srv)
{
    struct pollfd *pfd = srv->pfd_list;
    int n_pfd = srv->max_players + 1;

    // client connection
    if (pfd[0].revents & POLLIN)
        NEVENT_Accept(srv);

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
            NEVENT_Disconnect(srv, cl);
        }

        if (pfd[i].revents & POLLIN)
        {
            // Handle client data
            printf("SERVER: Received data from client on fd %d\n", pfd[i].fd);
            if (NEVENT_Read(cl) == PACKET_ERROR)
                NEVENT_Disconnect(srv, cl);
        }
    }
}
