#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "../include/cfg.h"
#include "../include/server.h"
#define PROP_MAX 100

/*

   Current issue leading to segfault is that we haven't parsed an existing file
 */
int main(void){	
	//Parse Config
	Config *cfg = init_cfg(); 

	//Pass the config to the server
	Server *server = init_server(cfg);
	start_server(server);

	free_cfg(cfg);
	return 0;	
}
