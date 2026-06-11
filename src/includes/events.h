#ifndef EVENTS_H
#define EVENTS_H
#include "packet.h"

typedef struct server server;
typedef struct client client;

#define CLIENT_BUFFER_SIZE 1024

void EVENT_Handle(server *srv);
int EVENT_Accept(server *srv);
int EVENT_Read(client *cl);
void EVENT_Disconnect(server *srv, client *cl);

#endif