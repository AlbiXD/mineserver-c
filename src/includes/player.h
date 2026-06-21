#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
typedef struct
{

    double X;
    double Y;
    double stance;
    double Z;

} player_position_t;

typedef struct
{
    double yaw;
    double pitch;
} player_look_t;

typedef struct
{
    uint8_t username[17];
    player_position_t position;
    player_look_t look;
    uint8_t on_ground;
} player_t;

#endif