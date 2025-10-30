#include "../include/cfg.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

char* cfg = "server.properties";

Config* init_cfg(){
	int fd;
	Config* cfp = NULL;

	if ((fd = open(cfg, O_WRONLY | O_CREAT | O_EXCL, 0644)) >= 0) {
		write_default(fd);
		printf("Creating new server configs...\n");
		cfp = (Config *)  malloc(sizeof(Config));
		cfp->host = "localhost";
		cfp->port = 25565;
	} else if (errno == EEXIST) {
		fd = open(cfg, O_RDONLY);
		printf("Loading existing server configs...\n");
	} else {
		printf("Failed to load server config\n");
		exit(1);
	}
	
	close(fd);
	return cfp; 	
}

void parse_cfg(int fd){

	return 0;
}
void write_default(int fd){
	write(fd, "ip=localhost\n", sizeof("ip=localhost\n")-1);
	write(fd, "port=25565\n", sizeof("port=25565\n")-1);
}
