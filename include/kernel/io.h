#ifndef IO_H
# define IO_H


# include <stdint.h>
# include <stddef.h>

# include <stdarg.h>

# define UINT_MAX 4294967295
# define INT_MAX 2147483647
# define INT_MIN -2147483648


//  I/O port operations


// Read a byte from a port
static inline uint8_t		inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ("inb %w1, %b0" : "=a"(ret) : "d"(port));
    return (ret);
}

// Write a byte to a port
static inline void			outb(uint16_t port, uint8_t val)
{
    asm volatile ("outb %b0, %w1" : : "a"(val), "d"(port));
}

// Write a word to a port
static inline void			outw(uint16_t port, uint16_t val)
{
    asm volatile ("outw %w0, %w1" : : "a"(val), "d"(port));
}

// I/O wait - small delay
static inline void			io_wait(void)
{
    outb(0x80, 0);
}

// Formatted output to terminal
int	ft_printf(const char *s, ...);

#endif
