#ifndef CFG_H
#define CFG_H

#include <unistd.h>

typedef struct Config {
	char host[16];
	unsigned short port;
} Config;


/**
 * @brief Performs the scraping or initialization of the configuration.
 *
 * @return Pointer to a Config structure allocated on the heap.
 *         The caller is responsible for freeing it with cfg_free().
 */
Config* init_cfg(void);

	
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
