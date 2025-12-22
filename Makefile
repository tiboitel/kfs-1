# Directories
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Compiler and linker
CFLAGS := -m32 -ffreestanding -fno-builtin -nostdlib -O2 \
	  -Wall -Wextra -Werror -Iinclude
CC := gcc $(CFLAGS) 
LD := ld -m elf_i386 -T linker.ld
AS := nasm -f elf32

# Source files
ASM_SRCS := $(shell find $(SRC_DIR) -name '*.s')
C_SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := \
	$(ASM_SRCS:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o) \
	$(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Output
KERNEL := $(BUILD_DIR)/kernel.bin

# Rules
all: $(KERNEL)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@

$(KERNEL): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(LD) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

