#ifndef TERMINAL_H
#define TERMINAL_H


#include "server.h"
#include "client.h"
#define TBUFF_SIZE 128




typedef enum
{
    SAY,
    HELP,
    UNKNOWN,
} Command;

void handle_terminal_event(Server* server);
int command_handler(Command cmd, char *buffer, Server* server);
int say_command(char *buffer, Server* server);

#endif