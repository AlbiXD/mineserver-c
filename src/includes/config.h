#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_OPTIONS 3
#define LINE_BUFFER 128
typedef struct{

    char        ip_address[16];    //IP Address
    in_port_t   port;              //Server Port
    int max_players;

    uint8_t is_valid; 


} config;


/*
 * Reads config from disk (server.properties). Falls back to
 * init_default_config() if the file is missing or malformed.
 */
void init_config(config *cfg);

/*
 * Populates cfg from the loaded
 * config file. Expects init_config() to have run first.
 */
void parse_config(config *cfg);

/*
 * Fills cfg with hardcoded fallback values:
 *   ip_address = "127.0.0.1"
 *   PORT       = 8080
 */
void init_default_config(config *cfg);

#endif