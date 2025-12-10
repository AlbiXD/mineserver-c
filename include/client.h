#ifndef CLIENT_H
#define CLIENT_H

#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#define BUFSIZE 128
typedef enum {
    STATE_NONE,
    STATE_HANDSHAKE_START,
    STATE_HANDSHAKE_REPLY,
    STATE_LOGIN_START,
    STATE_LOGIN_SUCCESS,
    STATE_SPAWN,
    STATE_PLAYING,
    STATE_DISCONNECTED
} ClientState;

typedef struct{    
    char usrname[16];
    unsigned char packet_buf[BUFSIZE];
    int cfd;
    int pfd_index;
    int client_index;
    ClientState state;
    struct sockaddr_in client_addr;
} Client;


int add_client(struct pollfd* pfd, Client* client, int pfd_n, int client_n, int cfd, struct sockaddr_in* client_addr);

#endif