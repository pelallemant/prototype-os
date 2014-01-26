#include <prototype-os/boot/bootloader/x86/asm.h>
#include <prototype-os/types.h>

extern void scrollup(unsigned int);
extern void print(char *);

extern kY;
extern kattr;

void main();
void start_next();

void init_gdt(void);

void _start(void)
{
  scrollup(25); // we remove all messages
  kY = 0;
  kattr = 0x5F;
  
  /* initialisation de la GDT et des segments */
  init_gdt();

  /* Initialisation du pointeur de pile %esp, ne peut pas être mis dans init_gdt à cause de leave qui 
    change la valuer de %esp par %ebp
  */
  asm("   movw $0x18, %ax \n \
          movw %ax, %ss \n \
          movl $0x20000, %esp");

  // calling a new function create a new stack frame, so %esp is saved (gcc use %esp when you call functions 
  // with parameters and it would be a problem)

  start_next();
}

// --------------------------


void task1(void)
{
	char *msg = (char *) 0x40001000;
	int i;

	msg[0] = 't';
	msg[1] = 'a';
	msg[2] = 's';
	msg[3] = 'k';
	msg[4] = '1';
	msg[5] = '\n';
	msg[6] = 0;

	while (1) {
		asm("mov %0, %%ebx; mov $0x01, %%eax; int $0x30":: "m"(msg));
		for (i = 0; i < 1000000; i++);
	}

	return;			/* never go there */
}

void task2(void)
{
	char *msg = (char *) 0x40001000;
	int i;

	msg[0] = 't';
	msg[1] = 'a';
	msg[2] = 's';
	msg[3] = 'k';
	msg[4] = '2';
	msg[5] = '\n';
	msg[6] = 0;

	while (1) {
		asm("mov %0, %%ebx; mov $0x01, %%eax; int $0x30":: "m"(msg));
		for (i = 0; i < 1000000; i++);
	}

	return;			/* never go there */
}

// --------------------------

void start_next()
{
  print("kernel : gdt loaded\n");

  // init interrupts  
  init_idt();
	print("kernel : idt loaded\n");

	init_pic();
	print("kernel : pic configured\n");

  /* Initialisation du TSS */
	asm("	movw $0x38, %ax \n \
		ltr %ax");
	print("kernel : tr loaded\n");

	init_mm();
	print("kernel : paging enable\n");

	move_cursor(-1, -1); // hide cursor

  load_task((u32 *) 0x100000, (u32 *) task1, 0x2000);
	load_task((u32 *) 0x200000, (u32 *) & task2, 0x2000);

  asm_sti; // re-activate interrupts

  main();
}

