TARGET := ../tog

TEST := ../test
CC := gcc
CFLAGS := -O2  -g -std=gnu17 -Wall -Wextra -Wpedantic -Wno-unused-parameter

SRCS := entities.c graph.c movement.c rules.c misc.c attack.c

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(CLIBS)


$(TEST): $(SRCS) main_test.c
	$(CC) $(CFLAGS) $^ -o $(TEST) $(CLIBS)

