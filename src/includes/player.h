#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
typedef struct
{

    double X;
    double Y;
    double stance;
    double Z;
    double yaw;
    double pitch;
    uint8_t onGround;

} player_position_t;

typedef struct
{
    uint8_t username[17];
    player_position_t position;

} player_t;



#endif