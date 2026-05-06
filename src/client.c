#include "./includes/client.h"



void init_client_list(client *clients, int max_players)
{
    for (int i = 0; i < max_players; i++) {
        clients[i].is_used = 0;
    }
}