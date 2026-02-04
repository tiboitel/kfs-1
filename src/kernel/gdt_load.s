bits 32

; Load GDT and update segment registers
section .text
global load_gdt

load_gdt:
	mov eax, [esp + 4]    ; Get parameter address
	lgdt [eax]            ; Load GDT
	
	; Load data segment
	mov ax, 0x10          ; Kernel data segment (index 2)
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax            ; Stack segment
	
	; Far jump to reload CS
	jmp 0x08:.reload_cs   ; Kernel code segment (index 1)
	
.reload_cs:
	ret
