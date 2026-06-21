#include "../includes/cmd_queue.h"

game_command_t *CMDQ_Pop(cmd_queue *q)
{

    // If the head is empty
    if (q->head == NULL)
        return NULL;

    game_command_t *cmd;
    q_node *tmp = q->head;

    cmd = tmp->cmd;
    // if the head->next points to NULL
    if (q->head->next == NULL)
        q->tail = NULL;

    q->head = q->head->next;

    // if head->next points to smt else
    free(tmp);
    return cmd;
}

void CMDQ_Push(cmd_queue *q, game_command_t *cmd)
{

    q_node *ptr = malloc(sizeof(q_node));

    ptr->next = NULL;
    ptr->cmd = cmd;

    if (q->head == NULL)
    {
        q->head = ptr;
        q->tail = ptr;
        return;
    }

    // tail points to last node
    q_node *tmp = q->tail;
    q->tail = ptr;
    tmp->next = ptr;
}