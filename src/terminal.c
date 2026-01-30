#include "../include/terminal.h"
/* Make terminal input single threaded ? Or 2 threads? */

/*
    Usage: /say This is a server wide announcment system
    @param
        Terminal Buffer for message parsing
    @return
        Will return 1 if error with command
        Will return 0 if successful
*/

int say_command(char *buffer, Server *server)
{
    // <SERVER>: 
    short send_len = strlen(buffer)-1;

    printf("%d\n", send_len);
    int s = (send_len*2)+3;
    unsigned char send_buff[s];

    memset(send_buff, 0, s);

    unsigned char* ptr = (unsigned char*) &send_len;

    send_buff[0] = 0x03;
    send_buff[1] = *(ptr+1);
    send_buff[2] = *(ptr);

    int sbi = 4;
    for(int i = 0; i < send_len; i++){
        send_buff[sbi] = buffer[i];
        sbi+=2;
    }


    send_packet(send_buff, server, s);

    return 0;
}

/*
    Will return 1 if error with command
    Will return 0 if successful
*/
int command_handler(Command cmd, char *buffer, Server *server)
{
    switch (cmd)
    {
    case SAY:
        say_command(buffer, server);
        break;
    default:
        break;
    }
    return 0;
}

void handle_terminal_event(Server *server)
{
    char TERMINAL_BUFFER[TBUFF_SIZE];
    // Listen for incoming arguments


        Command cmd;
        // READ INPUT
        int r = read(0, TERMINAL_BUFFER, TBUFF_SIZE);
        TERMINAL_BUFFER[r] = 0;

        // PARSE INPUT FOR COMMANDS
        char *token = strtok(TERMINAL_BUFFER, " ");
        char *args = token;
        // AUTHENTICATE INPUT
        if (strcmp(token, "/say") == 0)
        {
            args = args+5;
            cmd = SAY;
        }
        else
        {
            cmd = UNKNOWN;
            printf("Unknown command type ? for list of\n");
        }

        // RUN COMMAND
        if (cmd != UNKNOWN)
            command_handler(cmd, args, server);
}
