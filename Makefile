CFLAGS = -Wall -Wextra
CC = gcc

server: build/cfg.o build/main.o
	$(CC) $(CFLAGS) build/cfg.o build/main.o -o server
build/cfg.o: src/cfg.c
	$(CC) $(CFLAGS)  -c src/cfg.c -o build/cfg.o
build/main.o: src/server.c 
	$(CC) $(CFLAGS)  -c src/server.c -o build/main.o

