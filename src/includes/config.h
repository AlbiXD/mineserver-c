#ifndef CONFIG_H
#define CONFIG_H

#include <netinet/in.h>
#include <stdint.h>

#define CONFIG_OPTIONS 2
#define LINE_BUFFER 128
typedef struct{

    char        ip_address[16];    //IP Address
    in_port_t   PORT;              //Server Port


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