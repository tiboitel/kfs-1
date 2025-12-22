; 'Magic' constant for Grub to detect kernel's location.
%define MB_MAGIC 0x1BADB002

; Flags tells GRUB to load modules on page boundaries and provide a memory map.
; Bit 0: align modules on page boundaries
; Bit 1: request memory map from GRUB
; Multiboot provides ONLY what we explicitly request.
%define MB_FLAGS 0x00000003

; Calculate a cheksum that includes all the previous values.
%define MB_CHECKSUM (0 - (MB_MAGIC + MB_FLAGS))

; Start section of the executable that contain our Multiboot header.
section .multiboot
    ; Aligned following data on a multiple of 4 bytes.
    align 4
    ; Use previously declared constants.
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

; Section who contains data initalised to zeroes after kernel loaded.
; Stack is allocated in .bss.
; This is safe at early boot because:
; - paging is disabled
; - memory is identity-mapped by GRUB
; Later, stack should move to a dedicated region.
section .bss
    ; Aligned following data on a multiple of 16 bytes.
    align 16
    ; Allocation of a stack of 4 Kilobytes for our C code.
    stack_bottom:
        resb 4096
    stack_top:

; Section who contains assembly code to run after kernel loaded.
section .text
    ; Declaration of 'start' label as global (accessible outside this file)
    ; Needed by linker.ld
    global _start
    ; Declaration of 'kernel_main' label as external to this file.
    ; kernel_main: name of the main C function 'kernel.c'
    extern kernel_main

    _start:
        ; Set up enviroment to run C code.
        ; On x86, stack grows downward. So, start on top.
        ; Set stack pointer to the top of the stack.
	mov esp, stack_top
	and esp, 0xFFFFFFF0    ; 16-byte alignment for C ABI

	; Call our main function.
	call kernel_main

    hang:
        ; If the kernel's C code returns, we hang to the CPU.
        ; Disable CPU interrupts.
        cli
        ; Halt the CPU.
    .halt:
        hlt
        ; If it's fail, loop and try again.
        jmp .halt

