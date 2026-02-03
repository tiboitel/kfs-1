
# Docker configuration
DOCKER_IMAGE := i386-kernel-env
DOCKER_RUN 	 := docker run --rm -v $(shell pwd):/workspace -w /workspace $(DOCKER_IMAGE)

# Directories
SRC_DIR 	:= src
BUILD_DIR 	:= build
OBJ_DIR 	:= $(BUILD_DIR)/obj

# Compiler and linker (used inside Docker)
CFLAGS		:= -Wall -Wextra -Werror -Iinclude
CPPFLAGS 	:= -m32 -ffreestanding -fno-builtin -nostdlib -O2
CC 			:= gcc $(CFLAGS) $(CPPFLAGS)
LD 			:= ld -T linker.ld -m elf_i386
AS 			:= nasm -f elf32

# Source files
ASM_SRCS 	:= $(shell find $(SRC_DIR) -name '*.s')
C_SRCS 		:= $(shell find $(SRC_DIR) -name '*.c')
OBJS 		:= \
			   $(ASM_SRCS:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o) \
			   $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Output
KERNEL 		:= $(BUILD_DIR)/kernel.bin
ISO 		:= kernel.iso

# Rules
all: docker-run

# Build Docker image if it doesn't exist
docker-image:
	@docker images $(DOCKER_IMAGE) | grep -q $(DOCKER_IMAGE) || docker build -t $(DOCKER_IMAGE) .

# Build kernel inside Docker
docker-build: docker-image
	$(DOCKER_RUN) make local-build

# Build ISO inside Docker
docker-iso: docker-image
	$(DOCKER_RUN) make local-iso

# Run QEMU with the ISO
docker-run: docker-iso
	qemu-system-i386 -cdrom $(ISO)

# Local build rules (executed inside Docker container)
local-build: $(KERNEL)

local-iso: $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o $(ISO) iso

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

fclean: clean
	docker rmi -f $(DOCKER_IMAGE) 2>/dev/null || true

re: clean all

.PHONY: all clean fclean re docker-image docker-build docker-iso docker-run local-build local-iso

