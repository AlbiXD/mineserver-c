#ifndef CLIENT_H
#define CLIENT_H

#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFSIZE 128

typedef struct{    
    char usrname[16]; // Minecraft username
    unsigned char instream[BUFFSIZE]; // input stream (many packets)
    unsigned char packet[BUFFSIZE]; // current packet
    unsigned char pidx; // packet id
    int instream_len; //input stream size
    int cfd; // client socket descriptor
    int pfd_index; // pollfd index
    int client_index; // client index
    struct sockaddr_in client_addr; // client's information such as address, port
} Client;


int add_client(struct pollfd* pfd, Client* client, int pfd_n, int client_n, int cfd, struct sockaddr_in* client_addr);

#endif