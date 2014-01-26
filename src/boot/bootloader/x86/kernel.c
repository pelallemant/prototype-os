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

void start_next()
{
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

  main();
}

