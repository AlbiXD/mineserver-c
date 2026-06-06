#include "../includes/protocol.h"
#include <stdio.h>

// Will return the size in bytes of the packet or a -1 if packet is incomplete
int parse_handshake(uint8_t *client_buffer, uint8_t *packet_id ,size_t bufflen, size_t bytes_read){
    int size = 0;

    printf("Handling handshake\n");
    if(bytes_read < 3){
        printf("Partial Packet\n");        
        return 0;
    }
    
    size = 3 + client_buffer[2] * 2;

    printf("%d\n", size);

    if(bytes_read < size) return 0;


    return size;
}