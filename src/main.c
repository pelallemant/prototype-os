#include <prototype-os/boot/bootloader/x86/asm.h>
#include <prototype-os/types.h>

void show_cursor(void);
void move_cursor(u8 x, u8 y);

extern void scrollup(unsigned int);
extern void print(char *);

extern kY;
extern kattr;

void main()
{
  sti;

  kattr = 0x5E;
  print("Here is the main !\n");

  kattr = 0x4E;
  print("Hello world\n");

  show_cursor();
  
  while (1);
}

