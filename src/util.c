#include "../include/util.h"

int get_line(int fd, char *buf, int max_len)
{
    char c;
    int i = 0;
    while (read(fd, &c, 1) > 0){
        if (max_len == i){
            printf("Line is too long!\n");
            exit(1);
        }
        buf[i] = c;
        if (c == '\n'){
            buf[i] = 0;
            break;
        }
        i++;
    }
    return i;
}
