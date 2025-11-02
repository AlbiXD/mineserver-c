#ifndef CFG_H
#define CFG_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "util.h"
#define MAX_LINE_LEN 32
typedef struct Config {
	char* host;
	unsigned short port;
	int max_players;
} Config;


/**
 * @brief Performs the scraping or initialization of the configuration.
 *
 * @return Pointer to a Config structure allocated on the heap.
 *         The caller is responsible for freeing it with cfg_free().
 */
Config* init_cfg(void);

/**
 * @brief Parse config data from a file descriptor.
 *
 * Reads key=value pairs (e.g., "host=127.0.0.1") from the given fd
 * and loads them into a Config struct.
 *
 * @param fd Open file descriptor to read from.
 * @param cfg A pointer to the Config structure to store stuff.
 * @return 0 on success, <0 on failure.
 */
int parse_cfg(int fd, Config* cfg);
	
void free_cfg(Config * cfg);

/**
 * @brief Writes a default configuration to the given file descriptor.
 *
 * This function outputs a default configuration template or fallback values
 * to the provided file descriptor. Intended for first-time initialization
 * or when a config file does not exist.
 *
 * @param fd File descriptor to write to (must be valid and writable).
 */
void write_default(int fd);


#endif
