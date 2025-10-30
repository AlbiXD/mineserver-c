#include "../include/cfg.h"

char* cfg = "server.properties";

Config* init_cfg(){
	int fd;
	Config* cfp = (Config *)  malloc(sizeof(Config));

	if ((fd = open(cfg, O_WRONLY | O_CREAT | O_EXCL, 0644)) >= 0) {
		write_default(fd);
		printf("Creating new server configs...\n");
		cfp->host = "localhost";
		cfp->port = 25565;
	} else if (errno == EEXIST) {
		fd = open(cfg, O_RDONLY);
		printf("Loading existing server configs...\n");
		if(parse_cfg(fd, cfp)){
			printf("ERROR PARSING CONFIG CHECK FILE\n");
			exit(1);
		}
	}
	else {
		printf("Failed to load server config\n");
		exit(1);
	}

	close(fd);
	return cfp; 	
}

int parse_cfg(int fd, Config* cfg){
	char buf[MAX_LINE_LEN];
	while(get_line(fd, buf, MAX_LINE_LEN) > 0){
		
	}
	return 0;
}
void write_default(int fd){
	write(fd, "ip=localhost\n", sizeof("ip=localhost\n")-1);
	write(fd, "port=25565\n", sizeof("port=25565\n")-1);
}
