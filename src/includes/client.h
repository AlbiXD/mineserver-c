#include <netinet/in.h>

typedef struct{
    int client_fd;
    struct sockaddr_in client_addr;

} client;