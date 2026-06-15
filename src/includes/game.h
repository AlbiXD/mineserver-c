#ifndef GAME_H
#define GAME_H

#include <stdint.h>

typedef struct client client;

typedef struct
{
    uint8_t username[16];
} cmd_client_handshake_t;

typedef struct
{
    uint8_t username[16];
} cmd_client_login_t;

typedef union
{
    cmd_client_handshake_t handshake;
    cmd_client_login_t login;
} game_command_t;

typedef struct
{
    int id;
    client *sender;
    game_command_t command;
} command_t;

int GAME_CommandHandler(command_t *cmd);

#endif