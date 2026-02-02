#ifndef IO_H
# define IO_H


# include <stdint.h>
# include <stddef.h>

//// Define basic types if stdint.h not available
//typedef unsigned char		uint8_t;
//typedef unsigned short		uint16_t;
//typedef unsigned int		uint32_t;

// Read a byte from a port
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ("inb %w1, %b0" : "=a"(ret) : "d"(port));
    return ret;
}

// Write a byte to a port
static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %b0, %w1" : : "a"(val), "d"(port));
}

// I/O wait - small delay
static inline void io_wait(void)
{
    outb(0x80, 0);
}

#endif
