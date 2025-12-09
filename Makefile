CC = gcc
CFLAGS = -Wall -Wextra -g
OUTPUT = server.out

OBJ = build/util.o build/cfg.o build/main.o build/server.o build/client.o build/packet.o

server: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUTPUT)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
	rm -f $(OUTPUT)
