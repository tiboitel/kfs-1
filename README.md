
# KFS — A Tiny Kernel With Big Feelings

Welcome to **KFS**, a small educational x86 kernel built from scratch.  
It boots via GRUB, talks directly to VGA memory, handles a keyboard, and pretends to be a shell long enough for you to forget how peaceful user space was.

Kernel programming is equal parts power fantasy and existential dread. This project leans into both.

---

## Introduction

KFS is a freestanding 32-bit x86 kernel designed to run in QEMU.  
No libc. No OS. Just you, the CPU, and the creeping realization that *everything* is your problem now.

### Features

- Multiboot-compliant kernel (GRUB)
- Custom linker script
- Global Descriptor Table (GDT)
- VGA text-mode terminal
- Keyboard input (polling, scancodes, modifiers)
- Minimal `printf`
- Built-in shell commands
- Virtual terminals (multiple screens)
- Kernel panic screen (with ASCII art, obviously)

---

## Installation

### Requirements

- Docker
- QEMU (handled inside Docker, host only needs Docker)

### Build & Run

```bash
make
```

That’s it.
Docker builds the toolchain, compiles the kernel, generates an ISO, and boots it in QEMU.

If it doesn’t boot, it’s not Docker’s fault. It’s yours. (Or GRUB’s. Usually yours.)

### Cleanup
```
make clean     # remove build artifacts
make fclean    # also remove the Docker image
```

### How It Works

#### Boot Sequence

GRUB loads kernel.bin using Multiboot.

_start (assembly) sets up a stack and jumps to kernel_main.

Control enters C code. No safety net.

```
mov esp, stack_top
call kernel_main
```

Congratulations, you’re now the OS.

#### Memory & Linking

linker.ld places the kernel at 1MB.

Sections are page-aligned (4KB).

```
.multiboot header must stay near the start
```

This keeps GRUB happy and future-you slightly less angry.

#### GDT (Because x86 Demands Tribute)

The Global Descriptor Table defines memory segments:

- Kernel code / data / stack
- User code / data / stack (not fully used yet)

```
gdt[1].access = 0x9A; // kernel code
gdt[2].access = 0x92; // kernel data
```

Then lgdt is called, segment registers are reloaded, and suddenly protected mode feels real.

#### VGA Terminal

Direct writes to 0xB8000

80x25 text mode. (Mmh, nearly 4k)

Each cell = character + color byte (Mmh, fancy 😳)
```
vga_buffer[index] = (color << 8) | 'A';
```


Scrolling? Manual.
Cursor? Port I/O.
Comfort? Not included.

#### Keyboard Input

Polling via ports 0x60 / 0x64. Top-notch keyboard interface.

```
Raw scancodes → ASCII (US QWERTY)
```
Modifier keys tracked manually.

Supports:
```
Typing
Backspace / delete
Arrow movement
```

#### Shortcuts:
```
Alt + ← / → to switch virtual screens
```

Every key press reminds you why interrupts exist.

#### Shell & Commands

A tiny command handler lives in the kernel:

```help
clear
print stack
reboot
shutdown
setcolor <color>
```

Unknown commands are rejected politely, like a disappointed UNIX parent.

###  Virtual Screens

Up to 100 virtual terminals:
```
Each screen stores its own VGA buffer
Cursor state is preserved
Switch instantly with Alt + Left/Right
```
It feels like multitasking.
It isn’t. Don’t tell the kernel.

#### Panic Mode

When things go wrong (or you press Ctrl+D):

```
Cursor disabled
Screen cleared
ASCII art displayed
CPU halted forever
```

This is not a bug. This is closure. Don't talk to your toxic ex-kernel again.

### External ressources

https://wiki.osdev.org/Expanded_Main_Page

### Conclusion

KFS is not fast.
It is not safe.
It is not finished.

But it boots, prints, listens, and occasionally panics with dignity.

If kernel development ever feels overwhelming, remember:

> "The CPU does exactly what you tell it, The hard part is realizing what you actually told it to do." - *Morpheus.*

Have fun. Break things.
User space will still be there when you crawl back.

See you space, cowboy 🤠🔫

