TARGET := ../tog

DEBUG := ../togdbg

TEST := ../test
CC := gcc


DEBUG_FLAGS := -g -fsanitize=address -fsanitize=object-siz -fno-optimize-sibling-callse -fsanitize=undefined -fsanitize=leak -fsanitize=alignment
CFLAGS := -O1 -std=gnu17 -Wall -Wextra -Wpedantic -Wno-unused-parameter

SRCS := entities.c graph.c movement.c rules.c misc.c attack.c

all: $(TARGET)

$(TARGET):  $(SRCS) main.c
	$(CC) $(CFLAGS) $^ -o $(TARGET)
$(DEBUG) :   $(SRCS) main.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $^ -o $(DEBUG) 


$(TEST): $(SRCS) main_test.c
	$(CC) $(CFLAGS) $^ -o $(TEST) 

