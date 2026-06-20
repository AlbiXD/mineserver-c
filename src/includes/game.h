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

typedef struct
{
    double X;
    double Y;
    double stance;
    double Z;

    uint8_t onGround;

} cmd_client_position_t;

typedef struct
{
    double yaw;
    double pitch;
} cmd_client_look_t

    typedef union
{
    cmd_client_handshake_t handshake;
    cmd_client_login_t login;
    cmd_client_position_t position;
    cmd_client_look_t position;

} game_command_t;

typedef struct
{
    int id;
    client *sender;
    game_command_t command;
} command_t;

int GAME_CommandHandler(command_t *cmd);

#endif