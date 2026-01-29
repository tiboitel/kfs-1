# WARNING: If you want compile in 42, you need to replace 'grub2-mkrescue' by 'grub-mkrescue'

# Directories
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Compiler and linker
CFLAGS := -m32 -ffreestanding -fno-builtin -nostdlib -O2 \
	  -Wall -Wextra -Werror -Iinclude
CC := gcc $(CFLAGS) 
LD := ld -T linker.ld -m elf_i386
AS := nasm -f elf32

# Source files
ASM_SRCS := $(shell find $(SRC_DIR) -name '*.s')
C_SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := \
	$(ASM_SRCS:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o) \
	$(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Output
KERNEL := $(BUILD_DIR)/kernel.bin
ISO := kernel.iso

# Rules
all: run

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
	rm -rf iso $(ISO)

iso: $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	cp grub.cfg iso/boot/grub/
	grub2-mkrescue -o $(ISO) iso

run: iso
	qemu-system-i386 -cdrom $(ISO)

.PHONY: all clean iso run

