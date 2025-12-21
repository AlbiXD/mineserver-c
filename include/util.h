#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <poll.h>
#include "client.h"
#include <fcntl.h>
#include <time.h>



typedef struct { uint32_t cp; const char *ascii; } UTF_Map;



/**
 * Reads one line from a file descriptor.
 *
 * @param fd       File descriptor to read from.
 * @param buf      Buffer to store the line.
 * @param max_len  Max number of bytes to read.
 * @return Number of bytes read (0 on EOF).
 */
int get_line(int fd, char* buf, int maxlen);

int init_tables(struct pollfd* pfd, Client* clients, int pfd_n, int client_n);

void packet_dump(unsigned char *buf, int n);

#endif
