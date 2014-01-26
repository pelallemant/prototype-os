/*
  kernel.c
  It's the common kernel, 
  each CPU family has it's own implementation of the kernel in boot/bootloader/?/kernel.c
  which call this kernel.
*/

#include <prototype-os/boot/bootloader/x86/asm.h>
#include <prototype-os/types.h>
#include <prototype-os/kernel.h>

po_kernel __kernel;

void show_cursor(void);
void move_cursor(u8 x, u8 y);

extern void scrollup(unsigned int);
extern void print(char *);

extern kY;
extern kattr;

// the common kernel
void main()
{
  __kernel.mode = KERNEL_MODE_TEXT;

  asm_sti; // re-activate interrupts

  kattr = 0x5E;
  print("Here is the main !\n");

  kattr = 0x4E;
  print("Hello world\n");

  show_cursor();
  
  while (1);
}

