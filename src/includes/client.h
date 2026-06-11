#ifndef CLIENT_H
#define CLIENT_H
#include <netinet/in.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#define KB 1024
#define MULTIPLE 1

#define BUFFER_LENGTH KB * MULTIPLE

typedef struct server server;

/*
 * Represents a single connected client. is_used is 0 for empty slots, non-zero for active ones.
 */
typedef struct client {
    int client_fd;                  /* socket file descriptor; meaningful only when is_used != 0 */
    struct sockaddr_in client_addr; /* peer address, filled in by accept() */
    int is_used;                    /* 0 = slot is empty, 1 = slot holds an active client */
    int pfd_idx;                    /* index into the server's pfd_list array */
    int idx;                        /* index into the server's clients array */

    uint8_t client_buffer[BUFFER_LENGTH];
    size_t bytes_read;

    char username[16];
    int packet_len;

} client;

/*
 * Close all connected clients and mark their slots as unused.
 */
void CL_CloseAll(server *srv);

/*
 * Initialize an array of `max_players` client slots, marking each as unused.
 * Must be called once after allocating the array, before any other client
 * operations are performed.
 */
void CL_InitList(server *srv);

/*
 * Close a client's socket. Caller is responsible for also marking the
 * corresponding slot as unused (is_used = 0).
 */
void CL_Close(int client_fd);

/*
 * Find the first unused slot in the clients array.
 * Returns the slot index on success, or -1 if all slots are full.
 */
int CL_OpenSlot(client *clients, int max_players);

/*
 * Place an accepted client into slot `idx` of the server's clients array,
 * and register its socket in the corresponding pollfd slot for POLLIN.
 * Caller must ensure `idx` refers to an unused slot (see check_for_open_slots).
 */
void CL_Add(server *srv, int client_fd, struct sockaddr_in client_address, int idx);

/*
 * Reject a client connection that could not be admitted to the server.
 * Closes the socket immediately.
 * The client was never placed into the clients array, so no slot
 * bookkeeping is performed.
 */
void CL_Reject(int client_fd);

#endif