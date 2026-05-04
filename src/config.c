#include "./includes/config.h"
#include <unistd.h>
#include <stdio.h>



void init_default_config(config * cfg){
    printf("Config not found, generating new config...");
    return;
}

void init_config(config *cfg){

    //Check if config exists
    //if file exists
    if(access("server.properties", F_OK) == 0){
        init_default_config(cfg);
        return;
    }

    //If config exists parse

    //If it does not create default config


    return;
}