#include "../include/client.h"

int add_client(struct pollfd *pfd, Client *client, int pfd_n, int client_n)
{
    for (int i = 1; i < pfd_n; i++)
        if (pfd[i].fd == -1){
            
            client->pfd_index = i;
            break;
        }
    for (int i = 0; i < client_n; i++)
        if (client->cfd == -1){

            client->client_index = i;
            break;
        }
    return 0;
}