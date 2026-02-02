%define MB_MAGIC 0x1BADB002

%define MB_FLAGS 0x00000003

%define MB_CHECKSUM (0 - (MB_MAGIC + MB_FLAGS))

section .multiboot
    align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .bss
    align 16
    stack_bottom:
        resb 4096
    stack_top:

section .text
    global _start
    extern kernel_main

    _start:
	mov esp, stack_top
	and esp, 0xFFFFFFF0    ; 16-byte alignment for C ABI
	call kernel_main
    hang:
        cli
    .halt:
        hlt
        jmp .halt
