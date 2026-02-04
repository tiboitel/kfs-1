#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_ADDRESS 0x00000800
#define GDT_SIZE 7

// GDT entry structure
typedef struct __attribute__((packed)) gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_mid;
	uint8_t  access;
	uint8_t  limit_high:4;
	uint8_t  flags:4;
	uint8_t  base_high;
} gdt_entry;

// GDT pointer structure
typedef struct __attribute__((packed)) gdt_ptr {
	uint16_t limit;
	uint32_t base;
} gdt_ptr;

// GDT array
extern gdt_entry *gdt;

void gdt_init(void);
void print_stack(void);
void load_gdt(uint32_t);

#endif
