#include "../include/server.h"
#include "../include/cfg.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
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
        printf("Valid IP");
    }

    server->server_addr.sin_port = htons(cfg->port);
    server->port = cfg->port;
    server->cfg = cfg;
    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);

    return server;
}

void start_server(Server *server)
{
    printf("Starting Server on IP: %s:%d\n", server->cfg->host, server->port);
    if(bind(server->server_fd, (struct sockaddr *)&server->server_addr, sizeof(server->server_addr)) < 0){
        fprintf(stderr, "Failed to bind on port %d: %s\n", server->port, strerror(errno));
        //Call shut_down() perhaps?
        exit(1);
    }
     if(listen(server->server_fd, 5) < 0){
        fprintf(stderr, "Failed to listen on port %d: %s\n", server->port, strerror(errno));
        //Call shut_down() perhaps?
        exit(1);
     }

    printf("Server is now online\n");
    
    for(;;);
    
    return;
}

void server_stop(Server *server)
{
    return;
}
