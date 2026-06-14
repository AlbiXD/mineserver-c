#ifndef EVENTS_H
#define EVENTS_H
#include "packet.h"

typedef struct server server;
typedef struct client client;

#define CLIENT_BUFFER_SIZE 1024

void NEVENT_Handle(server *srv);
int NEVENT_Accept(server *srv);
int NEVENT_Read(client *cl);
void NEVENT_Disconnect(server *srv, client *cl);

#endif