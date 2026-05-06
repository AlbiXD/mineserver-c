#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <errno.h>
#include "server.h"
/*
 * Represents a single connected client. is_used is 0 for empty slots, non-zero for active ones.
 */
typedef struct
{
    int client_fd;                  /* socket file descriptor; meaningful only when is_used != 0 */
    struct sockaddr_in client_addr; /* peer address, filled in by accept() */
    int is_used;                    /* 0 = slot is empty, 1 = slot holds an active client */
} client;

/*
 * Initialize an array of `max_players` client slots, marking each as unused.
 * Must be called once after allocating the array, before any other client
 * operations are performed.
 */
void init_client_list(client *ptr, int max_players);

/*
 * Close a client's socket. Caller is responsible for also marking the
 * corresponding slot as unused (is_used = 0).
 */
void close_client(int client_fd);

/*
 * Handle I/O for a single connected client. Called after a successful accept()
 * once the client has been placed into a slot.
 * Returns 0 on normal completion, -1 on error.
 */
int handle_client_connect(server *srv);

/*
 * Find the first unused slot in the clients array.
 * Returns the slot index on success, or -1 if all slots are full.
 */
int check_for_open_slots(client *clients, int max_players);




/*
 * Reject a client connection that could not be admitted to the server.
 * Closes the socket immediately.
 * The client was never placed into the clients array, so no slot
 * bookkeeping is performed.
 */
void reject_client(int client_fd);

#endif