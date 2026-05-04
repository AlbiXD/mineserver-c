#ifndef CONFIG_H
#define CONFIG_H

#include <netinet/in.h>


typedef struct{

    char        ip_address[16];    //IP Address
    in_port_t   PORT;              //Server Port

} config;



void init_default_config(config *cfg);
void init_config(config *cfg);
void parse_config(config *cfg);
#endif