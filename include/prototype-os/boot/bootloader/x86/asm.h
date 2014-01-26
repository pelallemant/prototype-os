#ifndef __PROTOTYPE_OS__BOOT_BOOTLOADER_X86_ASM_H
#define __PROTOTYPE_OS__BOOT_BOOTLOADER_X86_ASM_H

/* desactive les interruptions */
#define asm_cli asm("cli"::)

/* reactive les interruptions */
#define asm_sti asm("sti"::)

/* ecrit un octet sur un port */
#define asm_outb(port,value) \
	asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value));

/* ecrit un octet sur un port et marque une temporisation  */
#define asm_outbp(port,value) \
	asm volatile ("outb %%al, %%dx; jmp 1f; 1:" :: "d" (port), "a" (value));

/* lit un octet sur un port */
#define asm_inb(port) ({    \
	unsigned char _v;       \
	asm volatile ("inb %%dx, %%al" : "=a" (_v) : "d" (port)); \
        _v;     \
})

#endif

