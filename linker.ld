/* Start execution at symbol designated as the entry point.
entrypoint: 'start' in start.s */

ENTRY(_start)

SECTIONS
{
	/* Begin putting sections at 1 Megabyte (1M).
	Memory below 1 Megabyte is reserved for other x86-related usage. */
	. = 1M;

	/* Align section in multiples of 4 Kilobytes (4K). 
	Useful for paging. */

	/* Multiboot header. Required to be near start of the executable. */
	/* multiboot_header: read-only section. */
	.multiboot BLOCK(4K) : ALIGN(4K)
	{
		KEEP(*(.multiboot))
	}

	/* Executable code. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	/* Read-only data: */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)	
	}

	/* Read-write data: (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data: (unitialized) and stack. */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
