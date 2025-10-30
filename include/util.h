#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>
#include <stdlib.h>

/**
 * Reads one line from a file descriptor.
 *
 * @param fd       File descriptor to read from.
 * @param buf      Buffer to store the line.
 * @param max_len  Max number of bytes to read.
 * @return Number of bytes read (0 on EOF).
 */
int get_line(int fd, char* buf, size_t maxlen);


#endif
