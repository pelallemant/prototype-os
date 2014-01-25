#ifndef __PROTOTYPE_OS__BOOT_BOOTLOADER_X86_IDT_H
#define __PROTOTYPE_OS__BOOT_BOOTLOADER_X86_IDT_H

#include <prototype-os/types.h>
#include <prototype-os/libc/string.h>

#define IDTBASE  0x800		/* addr. physique ou doit resider la IDT */
#define IDTSIZE  0xFF		/* nombre max. de descripteurs dans la table */

#define INTGATE  0x8E00		/* utilise pour gerer les interruptions */

/* Descripteur de segment */
struct idtdesc {
	u16 offset0_15;
	u16 select;
	u16 type;
	u16 offset16_31;
} __attribute__ ((packed));

/* Registre IDTR */
struct idtr {
	u16 limite;
	u32 base;
} __attribute__ ((packed));

struct idtr kidtr; /* Registre IDTR */
struct idtdesc kidt[IDTSIZE]; /* Table de IDT */

void init_idt_desc(u16, u32, u16, struct idtdesc *);
void init_idt(void);

#endif

