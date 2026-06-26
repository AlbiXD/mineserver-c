#ifndef GAME_H
#define GAME_H

#include <stdint.h>

typedef struct cmd_queue cmd_queue;
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
} cmd_client_look_t;

typedef union
{
    cmd_client_handshake_t handshake;
    cmd_client_login_t login;
    cmd_client_position_t position;
    cmd_client_look_t look;
} command_payload_t;

typedef struct
{
    int id;
    client *sender;
    command_payload_t payload;
} game_command_t;

int GAME_Tick(cmd_queue *queue);
int GAME_Login(game_command_t *cmd);
int GAME_Handshake(game_command_t *cmd);
int GAME_KeepAlive(game_command_t *cmd);

#endif