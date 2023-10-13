# Compilateur
CC := gcc

# Options de compilation
CFLAGS := -Wall -Wextra

# Nom de l'exécutable
TARGET := main

# Dossier des sources
SRC_DIR := src

# Liste des sources
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Compilation
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -f $(TARGET)
