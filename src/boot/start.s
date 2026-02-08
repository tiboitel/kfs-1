
; Magic value defined by Multiboot specification.
; GRUB scans the kernel binary for this exact number.
%define MB_MAGIC 0x1BADB002

; Multiboot flags.
; Bit 0 (0x1) : align loaded modules on page boundaries
; Bit 1 (0x2) : request memory information from the bootloader
%define MB_FLAGS 0x00000003

; Checksum required by Multiboot.
; GRUB use it to verifies header integrity.
%define MB_CHECKSUM (0 - (MB_MAGIC + MB_FLAGS))

section .multiboot
    align 4 ; Multiboot header must be 32-bit aligned.
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

; Block Starting Symbol
section .bss
    align 16
    stack_bottom:
        resb 4096 ; Reserve 4KB for kerne stack.
    stack_top:

section .text
    global _start ; Entry point symbol (used by linker)
    extern kernel_main ; C function to jump into

    _start:
	mov esp, stack_top
	and esp, 0xFFFFFFF0    ; 16-byte alignment for C ABI
	call kernel_main
    hang:
        cli
    .halt:
        hlt
        jmp .halt
