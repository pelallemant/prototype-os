extern void scrollup(unsigned int);
extern void print(char *);

extern kY;
extern kattr;

void main();

void init_gdt(void);

void _start(void)
{
  scrollup(200); // we remove all messages
  
  /* initialisation de la GDT et des segments */
  init_gdt();

  kY = 0;
  kattr = 0x5F;
  print("GDT Initialized !\n");

  
  /* Initialisation du pointeur de pile %esp */
  asm("   movw $0x18, %ax \n \
          movw %ax, %ss \n \
          movl $0x20000, %esp");

  main();
}
