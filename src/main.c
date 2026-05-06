#include <stdio.h>
#include "./includes/config.h"
#include "./includes/server.h"
#include "./includes/client.h"


int main()
{
    config cfg;
    server srv;

    // Parse config
    init_config(&cfg);

    // Initialize the server
    if (init_server(&srv, &cfg) < 0)
        return -1;


    start_server(&srv);


    stop_server(&srv);

    //Destroy server
    return 0;
}