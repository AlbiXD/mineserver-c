#include "./includes/config.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

void init_default_config(config *cfg)
{
    FILE *file;
    file = fopen("server.properties", "w");
    fprintf(file, "ip=localhost\n");
    fprintf(file, "port=25565\n");

    return;
}

void parse_config(config *cfg)
{
    FILE *f = fopen("server.properties", "r");
    char buffer[256];
    cfg->is_valid = 0;

    while (fgets(buffer, LINE_BUFFER, f))
    {
        char *token = strtok(buffer, "=");
        char *value = strtok(NULL, "=");

        if (strcmp(token, "ip") == 0)
        {
            for (int i = 0; i < LINE_BUFFER; i++)
                if (buffer[i] == '\n')
                    buffer[i] = 0;

            strcpy(cfg->ip_address, value);
            cfg->is_valid++;
        }
        else if (strcmp(token, "port") == 0)
        {
            cfg->PORT = (in_port_t)atoi(value);
            cfg->is_valid++;
        }
    }

    if (cfg->is_valid < CONFIG_OPTIONS)
    {
        printf("Config is missing fields regenerating from default...\n");
        return init_default_config(cfg);
    }

    return;
}

void init_config(config *cfg)
{

    // Check if config exists
    // if file exists
    if (access("server.properties", F_OK) != 0)
    {
        printf("Config not found, generating new config...\n");
        return init_default_config(cfg);
    }

    // If config exists parse
    printf("Config found parsing...\n");

    // If it does not create default config
    parse_config(cfg);

    return;
}