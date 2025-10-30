#include "../include/util.h"

/**
 * Reads one line from a file descriptor.
 *
 * @param fd       File descriptor to read from.
 * @param buf      Buffer to store the line.
 * @param max_len  Max number of bytes to read.
 * @return Number of bytes read (0 on EOF).
 */
int get_line(int fd, char *buf, size_t max_len)
{
    char c;
    int i = 0;
    while (read(fd, &c, 1) > 0)
    {
        if (max_len == i)
        {
            printf("Line is too long!\n");
            exit(1);
        }
        buf[i++] = c;
        if (c == '\n')
            break;
    }
    return i;
}
