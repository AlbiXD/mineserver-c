#ifndef CMD_QUEUE_H
#define CMD_QUEUE_H

#include "game.h"
#include <stddef.h>
#include <stdlib.h>
typedef struct q_node
{
    game_command_t *cmd;
    struct q_node *next;
} q_node;

typedef struct cmd_queue
{
    q_node *head;
    q_node *tail;
} cmd_queue;

void CMDQ_Init(cmd_queue *q);
void CMDQ_Push(cmd_queue *q, game_command_t *cmd);
game_command_t *CMDQ_Pop(cmd_queue *q);
#endif