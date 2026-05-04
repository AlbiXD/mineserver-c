#include <stdio.h>
#include "./includes/config.h"


int main(){
    config cfg;
    init_config(&cfg);
    printf("ip = %s", cfg.ip_address);
    printf("port = %d\n", cfg.PORT);

    return 0;
}