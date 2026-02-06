#include "kernel/gdt.h"

extern int ft_printf(const char *format, ...);

gdt_entry *gdt = (gdt_entry *)GDT_ADDRESS;

void	gdt_init(void)
{
	gdt_ptr gdtr;

	// Null descriptor
	gdt[0].limit_low = 0x0;
	gdt[0].base_low = 0x0;
	gdt[0].base_mid = 0x0;
	gdt[0].access = 0x0;
	gdt[0].limit_high = 0x0;
	gdt[0].flags = 0x0;
	gdt[0].base_high = 0x0;

	// Kernel Code Segment
	gdt[1].limit_low = 0xFFFF;
	gdt[1].base_low = 0x0;
	gdt[1].base_mid = 0x0;
	gdt[1].access = 0x9A;
	gdt[1].limit_high = 0xF;
	gdt[1].flags = 0xC;
	gdt[1].base_high = 0x0;

	// Kernel Data Segment
	gdt[2].limit_low = 0xFFFF;
	gdt[2].base_low = 0x0;
	gdt[2].base_mid = 0x0;
	gdt[2].access = 0x92;
	gdt[2].limit_high = 0xF;
	gdt[2].flags = 0xC;
	gdt[2].base_high = 0x0;

	// Kernel Stack Segment
	gdt[3].limit_low = 0xFFFF;
	gdt[3].base_low = 0x0;
	gdt[3].base_mid = 0x0;
	gdt[3].access = 0x96;
	gdt[3].limit_high = 0xF;
	gdt[3].flags = 0xC;
	gdt[3].base_high = 0x0;
	
	// User Code Segment
	gdt[4].limit_low = 0xFFFF;
	gdt[4].base_low = 0x0;
	gdt[4].base_mid = 0x0;
	gdt[4].access = 0xFA;
	gdt[4].limit_high = 0xF;
	gdt[4].flags = 0xC;
	gdt[4].base_high = 0x0;

	// User Data Segment
	gdt[5].limit_low = 0xFFFF;
	gdt[5].base_low = 0x0;
	gdt[5].base_mid = 0x0;
	gdt[5].access = 0xF2;
	gdt[5].limit_high = 0xF;
	gdt[5].flags = 0xC;
	gdt[5].base_high = 0x0;

	// User Stack Segment
	gdt[6].limit_low = 0xFFFF;
	gdt[6].base_low = 0x0;
	gdt[6].base_mid = 0x0;
	gdt[6].access = 0xF6;
	gdt[6].limit_high = 0xF;
	gdt[6].flags = 0xC;
	gdt[6].base_high = 0x0;

	// Prepare GDT pointer
	gdtr.limit = (sizeof(gdt_entry) * GDT_SIZE) - 1;
	gdtr.base = GDT_ADDRESS;

	// Load GDT
	load_gdt((uint32_t)&gdtr);
}

void	print_stack(void)
{
	uint32_t esp, ebp;
	uint32_t *stack;
	int i;

	__asm__ volatile ("mov %%esp, %0" : "=r"(esp));
	__asm__ volatile ("mov %%ebp, %0" : "=r"(ebp));

	ft_printf("\n=== Kernel Stack ===\n");
	ft_printf("ESP: 0x%x\n", esp);
	ft_printf("EBP: 0x%x\n\n", ebp);
	
	stack = (uint32_t *)esp;
	for (i = 0; i < 14; i++)
	{
		ft_printf("0x%x: 0x%x\n", (uint32_t)&stack[i], stack[i]);
	}
	ft_printf("\n");
}
