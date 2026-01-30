#include "../include/server.h"
#include "../include/terminal.h"
#include "../include/packet.h"

/*
    TODO: Packet Queue where every player packet gets worked on every 0.05s
    Create a queue on heap and pass to each player packet?

    TODO: Terminal thread must ensure
*/
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
    int pfd_n = server->max_players + 2;
    int max_clients = server->max_players;
    int *online = &server->online_players;

    struct pollfd pfd[pfd_n];
    Client clients[max_clients];
    server->clients = clients;

    init_tables(pfd, clients, pfd_n, max_clients); // Initialize tables

    pfd[0].fd = server->server_fd;
    pfd[0].events = POLLIN;

    // Set pfd for terminal input
    pfd[1].fd = 0; 
    pfd[1].events = POLLIN;

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

    struct timespec last;
    struct timespec now;

    int elapsed_ms = 0;
    int remaining = 50;
    for (;;) // Main server loop
    {
        clock_gettime(CLOCK_MONOTONIC, &last);

        int p = poll(pfd, pfd_n, remaining);
        // Listen for player connections
        if (pfd[0].revents & POLLIN)
        { // Player connection handle
            handle_new_connections(server, pfd, clients, pfd_n, max_clients);
        }


        // Handle terminal input
        if(pfd[1].revents & POLLIN){
            handle_terminal_event(server);
        }

        // Handle Client Events/I/O
        if (*online) // If no player is online why bother
        {
            handle_client_events(pfd, clients, max_clients, online);
        }

        clock_gettime(CLOCK_MONOTONIC, &now);

        long sec = now.tv_sec - last.tv_sec;
        long nsec = now.tv_nsec - last.tv_nsec;

        if (nsec < 0)
        {
            sec -= 1;
            nsec += 1000000000L;
        }

        elapsed_ms = sec * 1000 + nsec / 1000000;

        // printf("Time: %ldms\n", elapsed_ms);

        remaining = remaining - elapsed_ms; // How much time is left until next tick?

        if (remaining <= 0) // TICK
        {                   // if we have reached tick time
            // printf("TICK\n"); // print tick
            int overdue = -remaining;
            remaining = 50 - overdue;

            // Update the world
            if (*online)
            {
                for (int i = 0; i < *online; i++)
                {
                    unsigned char buf = 0x00;
                    write(clients[i].cfd, &buf, 1);
                }
            }
        }
    }

    return;
}



void handle_client_events(struct pollfd *pfd, Client *clients, int max_clients, int *online)
{
    for (int i = 0; i < max_clients; i++)
    { // Handle client stuff
        int p_index = clients[i].pfd_index;
        if (p_index == -1)
            continue;

        if (pfd[p_index].fd == -1)
            continue;

        if (pfd[p_index].revents & POLLIN)
        {
            int status = 0;
            // Handle Disconnect NEEDS REVISITING
            if ((status = packet_handler(&clients[i])) < 0)
            {
                // printf("Packet Handled = %d\n", status);
                disconnect_handler(&pfd[p_index], online, clients, i);
            }
        }
    }
}


void send_packet(unsigned char* buffer, Server* server, int packet_len){
    for(int i = 0; i < server->online_players; i++){
        int cfd = server->clients[i].cfd;
        if(cfd != -1){
            write(cfd, buffer, packet_len);
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

void handle_new_connections(Server *server, struct pollfd *pfd, Client *clients, int pfd_n, int max_clients)
{
    for (;;)
    {
        struct sockaddr_in client_addr;
        socklen_t cli_addr_len = sizeof(client_addr);

        int cfd = accept(server->server_fd,
                         (struct sockaddr *)&client_addr,
                         &cli_addr_len);

        if (cfd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            perror("accept");
            break;
        }

        // Accept until

        if (server->online_players >= server->max_players)
        {
            close(cfd);
            continue;
        }

        int flags = fcntl(cfd, F_GETFL, 0);
        fcntl(cfd, F_SETFL, flags | O_NONBLOCK);
        add_client(pfd, clients, pfd_n, max_clients, cfd, &client_addr);
        server->online_players++;
        // printf("Client Descriptor Created = %d\n", cfd);
    }
}

void disconnect_handler(struct pollfd *pfd, int *online, Client *c, int client_index)
{
    /*
    Oddly enough when a client disconnects, this runs a bit delayed but when put in packet handler it disconnects and connects just fine
    */
    close(pfd->fd);
    printf("Client Descriptor Closed: %d\n", pfd->fd);
    pfd->fd = -1;
    c->instream_len = 0;
    pfd->events = 0;
    pfd->revents = 0;
    c->pfd_index = -1;
    (*online)--;
}