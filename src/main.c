#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "../include/cfg.h"

#define PROP_MAX 100

/*

   Current issue leading to segfault is that we haven't parsed an existing file
 */
int main(){	
	Config* cfg = init_cfg();
	printf("%s\n", cfg->host);
	free(cfg);
	return 0;	
}
