CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC := $(shell find src -name "*.c")
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

server: $(OBJ)
	$(CC) $(OBJ) -o $@

build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build server

.PHONY: clean