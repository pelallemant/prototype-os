#include <prototype-os/drivers/process/x86/process.h>
#include <prototype-os/drivers/process/x86/mm.h>
#include <prototype-os/libc/string.h>

struct process p_list[32];
struct process *current = 0;
int n_proc = 0;

void load_task(u32 * code_phys_addr, u32 * fn, unsigned int code_size)
{
	u32 page_base, pages;
	u32 *pd;
	int i;

	/* Copie du code a l'adresse specifiee */
	memcpy((char *) code_phys_addr, (char *) fn, code_size);

	/* Mise a jour du bitmap */
	page_base = (u32) PAGE(code_phys_addr);

	if (code_size % PAGESIZE)
		pages = code_size / PAGESIZE + 1;
	else
		pages = code_size / PAGESIZE;

	for (i = 0; i < pages; i++)
		set_page_frame_used(page_base + i);

	/* Creation du repertoire et des tables de pages */
	pd = pd_create(code_phys_addr, code_size);

	/* Initialisation des registres */
	p_list[n_proc].pid = n_proc;
	p_list[n_proc].regs.ss = 0x33;
	p_list[n_proc].regs.esp = 0x40001000;
	p_list[n_proc].regs.cs = 0x23;
	p_list[n_proc].regs.eip = 0x40000000;
	p_list[n_proc].regs.ds = 0x2B;
	p_list[n_proc].regs.cr3 = (u32) pd;

	n_proc++;
}
