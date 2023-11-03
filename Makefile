# Options de compilation
CC := gcc
CFLAGS := -Wall -Wextra

# Nom de l'exécutable
TARGET := main

# Dossier des sources
SRC_DIR := src

# Liste des sources
SRCS := $(wildcard $(SRC_DIR)/*.c)

all: clean $(TARGET)

# Compilation
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f build/*.o $(TARGET)

.PHONY: clean