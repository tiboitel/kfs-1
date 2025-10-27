// Declaration of 'kernel_main' label as external to this file.
// kernel_main: name of the main C function 'kernel.c'
.extern kernel_main

// Declaration of 'start' label as global (accessible outside this file)
// Needed by linker.ld
.global start

// 'Magic' constant for Grub to detect kernel's location.
.set MB_MAGIC, 0x1BADB002
// Flags tells GRUB to load modules on page boundaries and provide a memory map.
.set MB_FLAGS, (1 << 0) | (1 << 1)
// Calculate a cheksum that includes all the previous values.
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))

// Start section of the executable that contain our Multiboot header.
.section .multiboot
	// Aligned following data on a multiple of 4 bytes.
	.align 4
	// Use previously declared constants.
	.long MB_MAGICS
	.long MB_FLAGS
	.long MB_CHECKSUM

// Section who contains data initalised to zeroes after kernel loaded.
.section .bss
	// Aligned following data on  a multiple of 16 bytes.
	.align 16
	// Allocation of a stack of 4 Kilobytes for our C code.
	stack_bottom:
		.skip 4096
	stack_top:

// Section who contains assembly code to run after kernel loaded.
.section .text
	// Set up enviroment to run C code.
	// On x86, stack grows downward. So, start on top.
	start:
		// Set stack pointer to the top of the stack.
		mov $stack_top, %esp
		// Call our main function.
		call kernel_main

		// If the kernel's C code returns, we hang to the CPU.
		hang:
			// Disable CPU interrupts.
			cli
			// Halt the CPU.
			hlt
			// If it's fail, loop and try again.
			jmp hang

