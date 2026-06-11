#include <stdio.h>
#include "./includes/config.h"
#include "./includes/server.h"
#include "./includes/client.h"


int main()
{
    config cfg;
    server srv;

    // Parse config
    CFG_Init(&cfg);

    // Initialize the server
    if (SV_Init(&srv, &cfg) < 0)
        return -1;


    SV_Start(&srv); // Enters main server loop


    SV_Stop(&srv);

    //Destroy server
    return 0;
}