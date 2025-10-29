#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define PROP_MAX 100

void write_cfg(int fd);
char* cfg = "server.properties";
struct server_node* ptr = NULL;
struct server_node{
	char *key;
	char *value;
	struct server_node* next;
};


int main(){
	
	int fd;

	if((fd = open(cfg, O_WRONLY | O_CREAT | O_EXCL, 0644)) >= 0){
		write_cfg(fd);
		printf("Creating new server configs...\n");
	}else if(errno == EEXIST){
		fd = open(cfg, O_RDONLY);
		printf("Loading existing server configs...\n");
	}else{
		printf("Failed to load server config\n");
		exit(1);
	}	
	return 0;	
}

void write_cfg(int fd){
	write(fd, "ip=localhost\n", sizeof("ip=localhost\n")-1);
	write(fd, "port=25565\n", sizeof("port=25565\n")-1);
}



