# ============================================
#   Makefile for kfs-1 kernel
# ============================================

# Project name
KERNEL_NAME := kernel

# Cross compiler prefix (you must have i386-elf-gcc and i386-elf-ld installed)
TARGET      := i386-elf
CC          := $(TARGET)-gcc
AS          := $(TARGET)-as
LD          := $(TARGET)-ld
OBJCOPY     := $(TARGET)-objcopy

# Directories
SRC_DIR     := .
BUILD_DIR   := build

# Files
C_SOURCES   := $(SRC_DIR)/kernel.c
ASM_SOURCES := $(SRC_DIR)/start.s
LINKER_SCRIPT := $(SRC_DIR)/linker.ld

# Output files
C_OBJS      := $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
ASM_OBJS    := $(ASM_SOURCES:$(SRC_DIR)/%.s=$(BUILD_DIR)/%.o)
OBJS        := $(ASM_OBJS) $(C_OBJS)
KERNEL_ELF  := $(BUILD_DIR)/$(KERNEL_NAME).elf
KERNEL_BIN  := $(BUILD_DIR)/$(KERNEL_NAME).bin

# Compiler and assembler flags
CFLAGS  := -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib -nostdinc -fno-pic -fno-builtin -fno-stack-protector
ASFLAGS := --32
LDFLAGS := -T $(LINKER_SCRIPT) -nostdlib -m elf_i386

# Default target
all: $(KERNEL_BIN)

# Rule: build binary kernel
$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "[OBJCOPY] $@"
	$(OBJCOPY) -O binary $< $@

# Rule: link ELF file
$(KERNEL_ELF): $(OBJS)
	@echo "[LD] $@"
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Rule: compile C source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "[CC] $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Rule: assemble ASM source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	@echo "[AS] $<"
	$(AS) $(ASFLAGS) -o $@ $<

# Clean build artifacts
clean:
	@echo "[CLEAN]"
	rm -rf $(BUILD_DIR)

# Optional: run in QEMU (if you have it installed)
run: all
	qemu-system-i386 -kernel $(KERNEL_BIN)

# Phony targets
.PHONY: all clean run

