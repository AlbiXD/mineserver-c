CC = gcc
CFLAGS = -Wall -Wextra -O2
BUILD = build
SRC = src

OBJS = $(BUILD)/main.o $(BUILD)/config.o $(BUILD)/server.o

all: server

server: $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf server $(BUILD)

.PHONY: all clean