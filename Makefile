
CC := gcc
CCFLAGS := -std=c11 -Wfatal-errors -Wall -Wextra -Os -s
LDFLAGS += -lm -lSDL2

OUTPUT := bin/engine
$(OUTPUT): $(wildcard src/*.c)
	$(CC) $(CCFLAGS) $^ -o $@ $(LDFLAGS)

