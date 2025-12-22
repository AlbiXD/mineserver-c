#ifndef CLIENT_H
#define CLIENT_H

#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFSIZE 128

typedef struct{    
    char usrname[16];
    unsigned char instream[BUFFSIZE];
    unsigned char packet[BUFFSIZE];
    unsigned char pidx;
    int instream_len;
    int cfd;
    int pfd_index;
    int client_index;
    struct sockaddr_in client_addr;
} Client;


int add_client(struct pollfd* pfd, Client* client, int pfd_n, int client_n, int cfd, struct sockaddr_in* client_addr);

#endif