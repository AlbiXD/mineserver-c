

#define CLIENT_BUFFER_SIZE 1024

typedef struct server server;
typedef struct client client;


/*
 * Handle read events for a client
 */
void handle_client_read(client* cl);

/*
 * Handle I/O for a single connected client. Called after a successful accept()
 * once the client has been placed into a slot.
 * Returns 0 on normal completion, -1 on error.
 */
int handle_client_connect(server *srv);


/*
 * Handle disconnect events for a client
 */
void handle_client_disconnect(client* cl);


