#ifndef __PROTOTYPE_OS__DRIVERS_PROCESS_X86_PROCESS_H
#define __PROTOTYPE_OS__DRIVERS_PROCESS_X86_PROCESS_H

#include <prototype-os/types.h>

struct process {
	unsigned int pid;

	struct {
		u32 eax, ecx, edx, ebx;
		u32 esp, ebp, esi, edi;
		u32 eip, eflags;
		u32 cs:16, ss:16, ds:16, es:16, fs:16, gs:16;
		u32 cr3;
	} regs __attribute__ ((packed));
} __attribute__ ((packed));

extern struct process p_list[];
extern struct process *current;
extern int n_proc;

void load_task(u32 *, u32 *, unsigned int);

#endif

