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

int init_tables(struct pollfd* pfd, Client* clients, int pfd_n, int client_n){
    for(int i = 0; i < pfd_n; i++){
        pfd[i].fd = -1;
        pfd[i].events = 0;
        pfd[i].revents = 0;
    }
    for(int i = 0; i < client_n; i++){
        clients[i].cfd = -1;
        clients[i].state = STATE_NONE;
        clients[i].pfd_index = 0;
    }
}


int packet_dump(int cfd){
    int fd = open("raw_packets.txt",  O_CREAT | O_WRONLY, 0644);
    char buf[BUFSIZ];

    time_t timer = time(NULL);
    struct tm* tm_info;

    tm_info = localtime(&timer);

    size_t len = strftime(buf, BUFSIZ, "%Y-%m-%d %H-%M-%S:", tm_info);
    ssize_t n = read(cfd, buf+len, BUFSIZ);
    ssize_t w = write(fd, buf, n+len);

    close(fd);

    return w;

}