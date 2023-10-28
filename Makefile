CC := gcc
CFLAGS := -Wall -g -Iinclude

SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

all: clean main

main: $(OBJ)
	@$(CC) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o

.PHONY: clean all