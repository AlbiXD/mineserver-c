#ifndef EVENTS_H
#define EVENTS_H

typedef struct server server;
typedef struct client client;

#define CLIENT_BUFFER_SIZE 1024

void handle_events(server *srv);

int handle_accept_event(server *srv);
int handle_read_event(client *cl);
void handle_disconnect_event(server *srv, client *cl);

#endif