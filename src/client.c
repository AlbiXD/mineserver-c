#include "../include/client.h"

int add_client(struct pollfd *pfd, Client *client, int pfd_n, int client_n, int cfd, struct sockaddr_in* client_addr){
    for (int i = 1; i < pfd_n; i++)
        if (pfd[i].fd == -1){
            
            client->pfd_index = i;
            pfd[i].fd = cfd;
            pfd[i].events = POLLIN;
            break;
        }
    for (int i = 0; i < client_n; i++)
        if (client->cfd == -1){
            client->client_index = i;
            client->cfd = cfd;
            client->client_addr = *client_addr;
            break;
        }
    return client->client_index;
}