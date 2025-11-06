CFLAGS = -Wall -Wextra -g
CC = gcc

server: build/util.o build/cfg.o build/main.o build/server.o build/client.o build/packet.o
	$(CC) $(CFLAGS) build/util.o build/cfg.o build/main.o build/client.o build/server.o build/packet.o -o server
build/cfg.o: src/cfg.c
	$(CC) $(CFLAGS)  -c src/cfg.c -o build/cfg.o
build/packet.o: src/packet.c
	$(CC) $(CFLAGS)  -c src/packet.c -o build/packet.o
build/client.o: src/client.c
	$(CC) $(CFLAGS)  -c src/client.c -o build/client.o
build/server.o: src/server.c
	$(CC) $(CFLAGS)  -c src/server.c -o build/server.o
build/util.o: src/util.c
	$(CC) $(CFLAGS) -c src/util.c -o build/util.o
build/main.o: src/main.c 
	$(CC) $(CFLAGS)  -c src/main.c -o build/main.o

