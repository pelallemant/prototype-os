#include <prototype-os/boot/bootloader/x86/asm.h>

void init_pic(void)
{
	/* Initialisation de ICW1 */
	asm_outb(0x20, 0x11);
	asm_outb(0xA0, 0x11);

	/* Initialisation de ICW2 */
	asm_outb(0x21, 0x20);	/* vecteur de depart = 32 */
	asm_outb(0xA1, 0x70);	/* vecteur de depart = 96 */

	/* Initialisation de ICW3 */
	asm_outb(0x21, 0x04);
	asm_outb(0xA1, 0x02);

	/* Initialisation de ICW4 */
	asm_outb(0x21, 0x01);
	asm_outb(0xA1, 0x01);

	/* masquage des interruptions */
	asm_outb(0x21, 0x0);
	asm_outb(0xA1, 0x0);
}

