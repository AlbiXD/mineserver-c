CFLAGS = -Wall -Wextra -g
CC = gcc

server: build/util.o build/cfg.o build/main.o build/server.o
	$(CC) $(CFLAGS) build/util.o build/cfg.o build/main.o build/server.o -o server
build/cfg.o: src/cfg.c
	$(CC) $(CFLAGS)  -c src/cfg.c -o build/cfg.o
build/server.o: src/server.c
	$(CC) $(CFLAGS)  -c src/server.c -o build/server.o
build/util.o: src/util.c
	$(CC) $(CFLAGS) -c src/util.c -o build/util.o
build/main.o: src/main.c 
	$(CC) $(CFLAGS)  -c src/main.c -o build/main.o

