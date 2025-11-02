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
    ClientState state;
    int pfd_index;
} Client;