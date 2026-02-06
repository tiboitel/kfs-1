#ifndef KEYBOARD_H
# define KEYBOARD_H

# include "io.h"

# define KEYBOARD_DATA_PORT	0x60
# define KEYBOARD_STATUS_PORT	0x64

// Scancodes for special keys
# define KEY_CTRL	0x1D
# define KEY_ALT	0x38
# define KEY_UP		0x48
# define KEY_DOWN	0x50
# define KEY_LEFT	0x4B
# define KEY_RIGHT	0x4D
# define KEY_DELETE	0x53
# define KEY_1		0x02
# define KEY_2		0x03
# define KEY_3		0x04
# define KEY_4		0x05

// Arrow key scancodes
#define KEY_UP		0x48
#define KEY_DOWN	0x50
#define KEY_LEFT	0x4B
#define KEY_RIGHT	0x4D
#define KEY_DELETE	0x53

void		keyboard_init(void);
void		keyboard_poll(void);

#endif