#include "../include/client.h"

int add_client(struct pollfd *pfd, Client *client, int pfd_n, int client_n)
{
    for (int i = 1; i < pfd_n; i++)
        if (pfd[i].fd == -1); // Assign client their pfd index
}