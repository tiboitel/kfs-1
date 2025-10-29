# Directories
SRC_DIR := .
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Compiler and linker
CC := gcc -m32 -ffreestanding -O2 -Wall -Wextra
LD := ld -m elf_i386 -Ttext 0x1000
AS := nasm -f elf32

# Source files
ASM_SRCS := $(SRC_DIR)/start.s
C_SRCS := $(SRC_DIR)/kernel.c
OBJS := $(OBJ_DIR)/start.o $(OBJ_DIR)/kernel.o

# Output
KERNEL := $(BUILD_DIR)/kernel.bin

# Rules
all: $(KERNEL)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	$(AS) $< -o $@

$(KERNEL): $(OBJS)
	$(LD) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

