#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

extern int global_id;

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
    player_position_t position;
    player_look_t look;
    uint8_t on_ground;
} player_location_t;

typedef struct
{
    int entity_id;
    uint8_t username[17];
    player_location_t loc;
} player_t;

#endif