#include "./includes/server.h"
#include "./includes/config.h"
#include "./includes/client.h"
#include "./includes/events.h"

int handle_client_connect(server *srv){
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

        printf("Client connected\n");
        int idx = check_for_open_slots(srv->clients, srv->max_players);

        // Terminate gracefully if no slots are open
        if (idx < 0)
        {
            reject_client(client_fd);
            continue;
        }

        add_client(srv, client_fd, client_address, idx);
    }
}



/// Handle read events for a client for a nonblocking client
int handle_client_read(client *cl){
    char buffer[CLIENT_BUFFER_SIZE];
    ssize_t bytes_read = 0;

    


}