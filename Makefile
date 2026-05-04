CC=gcc
OPTIONS= -c
BUILD=build
SRC=src

all: server

$(BUILD):
	mkdir -p $(BUILD)

server: config.o main.o
	$(CC) $(BUILD)/config.o $(BUILD)/main.o -o $@

main.o: $(SRC)/main.c $(BUILD)
	$(CC) $(OPTIONS) $< -o $(BUILD)/$@

config.o: $(SRC)/config.c $(BUILD)
	$(CC) $(OPTIONS) $< -o $(BUILD)/$@ 



clean:
	rm -rf server build
