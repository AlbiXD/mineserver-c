#include "../includes/server.h"
#include "../includes/config.h"
#include "../includes/client.h"
#include "../includes/events.h"

int SV_Init(server *srv, const config *cfg)
{

    *srv = (server){
        .cfg = cfg,
        .max_players = cfg->max_players,
        .clients = malloc(sizeof(client) * cfg->max_players),
        .pfd_list = malloc(sizeof(struct pollfd) * (cfg->max_players + 1)),
        .queue = {0},
    };

    if ((srv->server_fd = SOCKET_CreateListening(srv)) < 0)
        return -1;

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

    CL_InitList(srv); // Initializes the client list + pollfd list

    printf("SERVER: Starting on IP %s\n", cfg->ip_address);
    printf("SERVER: Running on PORT %hu\n", cfg->port);
    printf("SERVER: Server initialized!\n");

    return 0;
}

int SV_Start(server *srv)
{

    printf("SERVER: Server is now online and ready to accept clients\n");
    struct pollfd *pfd = srv->pfd_list;

    // Listening socket / server socket poll
    pfd[0] = (struct pollfd){.fd = srv->server_fd, .events = POLLIN | POLLHUP | POLLERR};

    int n_pfd = srv->max_players + 1;
    int pret = 0;

    int timeout = 50;

    struct timespec end, start;

    int ticks = 0;

    for (;;) // Main server loop
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        pret = poll(pfd, n_pfd, timeout);
        clock_gettime(CLOCK_MONOTONIC, &end);

        uint64_t total_ms =
            end.tv_sec * 1000LL +
            end.tv_nsec / 1000000LL;

        uint64_t start_ms =
            start.tv_sec * 1000LL +
            start.tv_nsec / 1000000LL;

        int delta_time = (int)total_ms - (int)start_ms;
        int remaining = timeout - delta_time;

        timeout = remaining;

        if (pret != 0)
        {
            NEVENT_Handle(srv);
        }

        if (timeout <= 0)
        {
            ticks++;
            timeout = 50;
            GAME_Tick(&srv->queue);

            for (int i = 0; i < srv->max_players; i++)
            {
                if (srv->clients[i].is_used == 0)
                    continue;
                game_command_t cmd = (game_command_t){.id = 0x0, .sender = &srv->clients[i]};
                GAME_KeepAlive(&cmd);
            }
        }

        if (ticks == 20) // printf("20 TPS HIT\n");
            ticks = 0;
    }

    return 0;
}

void SV_Stop(server *srv)
{

    printf("SERVER: Shutting down server...\n");
    // Need to free and close clients
    CL_CloseAll(srv);
    close(srv->server_fd); // Close Server
    free(srv->pfd_list);
    free(srv->clients);
    return;
}