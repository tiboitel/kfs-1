#ifndef KEYBOARD_H
# define KEYBOARD_H

# include "io.h"

# define KEYBOARD_DATA_PORT	0x60
# define KEYBOARD_STATUS_PORT	0x64

// Initialize keyboard
void			keyboard_init(void);

// Poll keyboard and handle input (polling mode)
void			keyboard_poll(void);

#endif