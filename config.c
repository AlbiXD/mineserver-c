#include "config.h"
#include <unistd.h>




config init_default_config(config * cfg){

}

void init_config(config *cfg){
    //Check if config exists
    //if file exists

    if(access(".", F_OK) == 0){
        init_default_config(cfg);
        return;
    }

    //If config exists parse

    //If it does not create default config


    return;
}